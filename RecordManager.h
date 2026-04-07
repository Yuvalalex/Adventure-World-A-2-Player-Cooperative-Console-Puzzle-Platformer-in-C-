#pragma once
#include <string>
#include <fstream>
#include <map>

class RecordManager {
private:
    std::string stepsFileName;
    std::string resultFileName;
    std::map<long, char> stepsMap;
    std::ofstream stepsOutFile;
	long lastRecordedTick = -1;
	long endTick = -1;
	long rngSeed = -1;

public:
    RecordManager();
    ~RecordManager();

    void setFiles(const std::string& baseName);
    void setStepsFileName(const std::string& stepFileName) { stepsFileName = stepFileName; };
    void setResultFileName(const std::string& resFileName) { resultFileName = resFileName; };

    void startWriting();
    void startReading();
    void closeFiles();

    void recordStep(long gameTick, char key);
    bool readStep(long gameTick, char& outKey);
    void logEvent(long gameTick, const std::string& message);
    void saveResult(int level, float time, int moves);

	bool isFinished() const { return stepsMap.empty(); }
	long getLastRecordedTick() const { return lastRecordedTick; }
	long getEndTick() const { return endTick; }
	long getRngSeed() const { return rngSeed; }
	const std::string& getResultFileName() const { return resultFileName; }
	void writeEndMarker(long gameTick);
	void writeSeedMarker(long seed);
	bool isReplayFullyConsumed(long currentTick) const;
    bool compareResults(const std::string& expectedFile);
};
