#include "Player.h"
#include "Screen.h"
#include "Utils.h"
#include "Move.h"
#include "BombObject.h"
#include "TorchObject.h"
#include "SpringObject.h"
#include "SwordObject.h"
#include <iostream>
#include <memory>

using namespace std;

Player::Player()
    : location(0, 0), startPosition(0, 0), initialPos(-1, -1),
    symbol(' '), id(0), color(Color::WHITE),
    lives(INITIAL_LIVES), score(0), keys(0),
    finished(false), goToPrevRoom(false),
    currentDir(Direction::STAY),
    isCompressingSpring(false), compressedSpringsCount(0),
    compressionDir(Direction::NONE), momentumDir(Direction::NONE),
    velocity(1), velocityDuration(0),
    _hasTorch(false), _hasBomb(false), _hasSword(false) {}

Player::Player(int x, int y, char c, int playerId, Color col)
    : location(x, y), startPosition(x, y), initialPos(x, y),
    symbol(c), id(playerId), color(col),
    lives(INITIAL_LIVES), score(0), keys(0),
    finished(false), goToPrevRoom(false),
    currentDir(Direction::STAY),
    isCompressingSpring(false), compressedSpringsCount(0),
    compressionDir(Direction::NONE), momentumDir(Direction::NONE),
    velocity(1), velocityDuration(0),
    _hasTorch(false), _hasBomb(false), _hasSword(false) {
    reset();
}

void Player::reset() {
    location = startPosition;
    resetToInitialPosition();

    lives = INITIAL_LIVES;
    score = 0;
    keys = 0;
    finished = false;
    goToPrevRoom = false;
    currentDir = Direction::STAY;

    _hasTorch = false;
    _hasBomb = false;
    _hasSword = false;

    resetLevelState();
}

void Player::resetLevelState() {
    // Reset physics and temporary states only 
    resetToInitialPosition();
    finished = false;
    goToPrevRoom = false;
    currentDir = Direction::STAY;
    isCompressingSpring = false;
    compressedSpringsCount = 0;
    activeSprings.clear();
    stopMomentum();
    resetMessages();
}

void Player::resetToInitialPosition() {
    if (initialPos.getX() != -1 && initialPos.getY() != -1) {
        setPosition(initialPos);
    }
}

bool Player::useKey() {
    if (keys > 0) {
        keys--;
        return true;
    }
    return false;
}

void Player::update(float deltaTime) {
    decreaseMomentumDuration();
}

void Player::draw() const {
    Utils::gotoxy(location.getX(), location.getY());
    Utils::setTextColor(color);
    cout << symbol;
    Utils::setTextColor(Color::WHITE);
}

ObjectType Player::handleDispose(Screen& screen, Player& otherPlayer) {  // smart pointer to prevent memory leak
    // Dispose bomb
    if (_hasBomb) {
       
        auto bomb = std::make_unique<BombObject>(location);
        bomb->startTicking();
        screen.dropObject(bomb.release());

        setMessage("Bomb Dropped!");
        _hasBomb = false;
        screen.updateVisuals();
        return ObjectType::BOMB;
    }
    // Dispose torch
    else if (_hasTorch) {
        auto torch = std::make_unique<TorchObject>(location);
        screen.dropObject(torch.release());

        setMessage("Torch Dropped.");
        _hasTorch = false;
        screen.updateStats(*this, otherPlayer);
        screen.reprintFullBoard(*this, otherPlayer);
        return ObjectType::TORCH;
    }
    // Dispose sword
    else if (_hasSword) {
        auto sword = std::make_unique<SwordObject>(location);
        screen.dropObject(sword.release());

        setMessage("Sword Dropped.");
        _hasSword = false;
        screen.updateVisuals();
        return ObjectType::SWORD;
    }

    setMessage("Nothing to dispose.");
    return ObjectType::NONE;
}

void Player::startCompressing(Direction dir) {
    if (!isCompressingSpring) {
        isCompressingSpring = true;
        compressedSpringsCount = 0;
        compressionDir = dir;
    }
}

void Player::stopCompressing() {
    isCompressingSpring = false;
    compressedSpringsCount = 0;
}

void Player::releaseSpring(Screen& screen) {
    if (activeSprings.empty()) {
        stopCompressing();
        return;
    }

    int springCount = static_cast<int>(activeSprings.size());

    // Restore visual appearance of compressed springs
    for (SpringObject* spring : activeSprings) {
        if (spring) {
            spring->resetVisuals();
            Utils::gotoxy(spring->getLoc().getX(), spring->getLoc().getY());

            // Restore visibility based on light conditions
            Utils::setTextColor(screen.isLightOn() ? spring->getColor() : Color::BLACK);
            cout << spring->getRepr();
            spring->setIsVisible(true);
        }
    }
    activeSprings.clear();

    // Calculate launch Opposite Direction
    Direction launchDirection = Move::getOppositeDirection(compressionDir);
    this->setDirection(launchDirection);

    Point newPosition = location;
    for (int i = 0; i < springCount; i++) {
        newPosition = Move::calculateNextPoint(newPosition, launchDirection);
    }

    if (screen.inBoard(newPosition)) {
        location = newPosition;
    }

    // Apply momentum based on compression count
    auto launchParams = SpringObject::calculateLaunchParams(springCount);
    applyMomentum(launchParams.first, launchParams.second, launchDirection);
    setMessage("BOING!");

    stopCompressing();
}

void Player::setMomentum(int vel, int dur, Direction dir) {
    velocity = vel;
    velocityDuration = dur;
    momentumDir = dir;
}

void Player::applyMomentum(int vel, int dur, Direction dir) {
    setMomentum(vel, dur, dir);
}

void Player::stopMomentum() {
    velocity = 1;
    velocityDuration = 0;
    momentumDir = Direction::NONE;
}

void Player::decreaseMomentumDuration() {
    if (velocityDuration > 0) {
        velocityDuration--;
    }
}

void Player::transferEnergyTo(Player& target) {
    if (velocityDuration <= 0) {
        return;
    }

    target.setMomentum(this->velocity, this->velocityDuration, this->momentumDir);
    this->stopMomentum();
    target.setMessage("Pushed!");
}