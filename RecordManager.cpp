#include "RecordManager.h"
#include "Constants.h"
#include <iostream>
#include <direct.h> 
#include <fstream>

// Constructor
RecordManager::RecordManager() {
}

// Destructor
RecordManager::~RecordManager() {
    closeFiles();
}

// Set the file names for recording
void RecordManager::setFiles(const std::string& baseName) {
    stepsFileName = baseName + ".steps.txt";
    resultFileName = baseName + ".result.txt";

    // Reset variables
    lastRecordedTick = -1;
    endTick = -1;
    rngSeed = -1;

    // Create a clean results file
    std::ofstream resFile(resultFileName, std::ios::out | std::ios::trunc);
    if (resFile.is_open()) {
        resFile << "--- Game Event Log: " << baseName << " ---" << std::endl;
        resFile.close();
    }
}

// Start writing mode - open steps file for writing
void RecordManager::startWriting() {
    stepsOutFile.open(stepsFileName, std::ios::out | std::ios::trunc);
}

// Start reading mode - load all steps from file
void RecordManager::startReading() {
    std::ifstream inFile(stepsFileName);
    if (!inFile.is_open()) {
        return;
    }

    std::string firstWord;
    while (inFile >> firstWord) {
        // Check for seed marker
        if (firstWord == "SEED") {
            long seed;
            if (inFile >> seed) {
                rngSeed = seed;
            }
            continue;
        }

        // Check for end marker
        if (firstWord == "END") {
            long tick;
            if (inFile >> tick) {
                if (tick > endTick) {
                    endTick = tick;
                }
            }
            continue;
        }

        // Normal line: tick and key
        long tick = std::stol(firstWord);
        int keyAsInt;
        if (!(inFile >> keyAsInt)) {
            break;
        }

        // Store the step
        stepsMap[tick] = static_cast<char>(keyAsInt);

        if (tick > lastRecordedTick) {
            lastRecordedTick = tick;
        }
    }

    inFile.close();
}

// Write the random seed to the steps file
void RecordManager::writeSeedMarker(long seed) {
    if (stepsOutFile.is_open()) {
        stepsOutFile << "SEED " << seed << std::endl;
        rngSeed = seed;
    }
}

// Write the end marker to the steps file
void RecordManager::writeEndMarker(long gameTick) {
    if (stepsOutFile.is_open()) {
        stepsOutFile << "END " << gameTick << std::endl;
        endTick = gameTick;
    }
}

// Check if all recorded steps have been consumed
bool RecordManager::isReplayFullyConsumed(long currentTick) const {
    // Check if there are still steps to read
    bool stepsDone = stepsMap.empty();
    if (!stepsDone) {
        return false;
    }

    // Determine the final tick
    long finalTick;
    if (endTick >= 0) {
        finalTick = endTick;
    }
    else {
        finalTick = lastRecordedTick;
    }

    if (finalTick < 0) {
        return true;
    }

    return currentTick >= finalTick;
}

// Record a player step (key press)
void RecordManager::recordStep(long gameTick, char key) {
    if (!stepsOutFile.is_open()) {
        return;
    }

    // Don't record ESC key
    if (key == 27) {
        return;
    }

    stepsOutFile << gameTick << " " << static_cast<int>(key) << std::endl;

    if (gameTick >= lastRecordedTick) {
        lastRecordedTick = gameTick;
    }
}

// Read a step for the given tick
bool RecordManager::readStep(long gameTick, char& outKey) {
    auto it = stepsMap.find(gameTick);

    if (it != stepsMap.end()) {
        outKey = it->second;
        stepsMap.erase(it);  // Remove to prevent infinite loop
        return true;
    }

    return false;
}

// Log an event to the results file
void RecordManager::logEvent(long gameTick, const std::string& message) {
    if (resultFileName.empty()) {
        return;
    }

    std::ofstream resFile(resultFileName, std::ios::app);
    if (resFile.is_open()) {
        resFile << "[Tick: " << gameTick << "] " << message << std::endl;
        resFile.close();
    }
}

// Save a result (level finished)
void RecordManager::saveResult(int level, float time, int moves) {
    logEvent(moves, "Level Finished. Time: " + std::to_string(time));
}

// Close all open files
void RecordManager::closeFiles() {
    if (stepsOutFile.is_open()) {
        stepsOutFile.close();
    }

    stepsMap.clear();
    lastRecordedTick = -1;
    endTick = -1;
    rngSeed = -1;
}

// Compare actual results with expected results
bool RecordManager::compareResults(const std::string& expectedFile) {
    // Create comparison log file
    std::ofstream log(SILENT_MODE_CMP_REPO_SCREEN_FILE_NAME);
    log << ">>> SILENT LOAD MODE: Replaying <<<" << std::endl;
    log << "Comparing results..." << std::endl;
    log << "Actual: " << resultFileName << std::endl;
    log << "Expected: " << expectedFile << std::endl;

    // Open both files
    std::ifstream actualFile(resultFileName);
    std::ifstream expectedFileStream(expectedFile);

    if (!actualFile.is_open() || !expectedFileStream.is_open()) {
        log << "Error opening files for comparison." << std::endl;
        return false;
    }

    std::string actualLine;
    std::string expectedLine;
    int lineNum = 1;

    // Skip first 2 lines (header)
    for (int i = 0; i < 2; i++) {
        std::getline(actualFile, actualLine);
        std::getline(expectedFileStream, expectedLine);
        lineNum++;
    }

    // Compare line by line
    while (true) {
        bool hasActual = (bool)std::getline(actualFile, actualLine);
        bool hasExpected = (bool)std::getline(expectedFileStream, expectedLine);


        // Both files ended - success
        if (!hasActual) {
            break;
        }

        // Compare lines
        if (actualLine != expectedLine) {
            log << "Mismatch at line " << lineNum << std::endl;
            log << " Actual:   " << actualLine << std::endl;
            log << " Expected: " << expectedLine << std::endl;
            log << "============= TEST NOT PASSED! =============" << std::endl;
            log << "Press any key to exit..." << std::endl;
            return false;
        }

        lineNum++;
    }

    log << "============= TEST PASSED! =============" << std::endl;
    log << "Press any key to exit..." << std::endl;

    return true;
}