// labpowerqt is a Gui application to control programmable lab power supplies
// Copyright © 2015, 2016 Christian Rapp <0x2a at posteo dot org>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SERIALQUEUE_H
#define SERIALQUEUE_H

#include <QVariant>
/*
 * We need a conditional variable to signal a waiting thread
 */
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>

#include <memory>
/*
 * We use a std::queue as basis for this threadsafe queue
 */
#include <queue>

#include "serialcommand.h"

/**
 * @brief Threadsafe queue that holds the SerialCommands that should be used with
 * the hardware
 */
class SerialQueue
{
public:
    SerialQueue();

    void push(int command, int channel = 1, const QVariant &value = QVariant(),
              bool withReply = false, int replyLength = 0);
    std::shared_ptr<SerialCommand> pop();

    bool empty();

private:
    std::queue<std::shared_ptr<SerialCommand>> internalQueue;
    QMutex qmtx;
    /**
     * @brief This conditional variable is used by the queue to notify waiting
     * threads that a new SerialCommand is ready to be popped.
     */
    QWaitCondition qcondition;
};

#endif  // SERIALQUEUE_H
