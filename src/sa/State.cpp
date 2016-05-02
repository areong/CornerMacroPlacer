#include "sa/State.h"

State::State() {
    startPosition = 0;
    backupPosition = -1;
}

State::~State() {

}

void State::setStartPosition(int startPosition) {
    this->startPosition = startPosition;
}

void State::setBackupPosition(int backupPosition) {
    this->backupPosition = backupPosition;
}

State *State::copy() {
    State *state = copyState();
    state->setStartPosition(backupPosition + 1);
    return state;
}

State *State::getBackup() {
    State *state = getBackupState();
    state->setStartPosition(backupPosition + 1);
    return state;
}