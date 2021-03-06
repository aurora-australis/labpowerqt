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

#ifndef RECORDAREA_H
#define RECORDAREA_H

#include <QDateTime>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>

#include "global.h"
#include "settingsdefinitions.h"
#include "switchbutton.h"

/**
 * @brief Widget that manages recordings in the main gui
 */
class RecordArea : public QWidget
{
    Q_OBJECT
public:
    explicit RecordArea(QWidget *parent = 0);

signals:

    /**
     * @brief Signal recording start / stop
     * @param status
     */
    void record(bool status, QString rname);

public slots:

    /**
     * @brief In case the recording was started / stopped from somewhere else
     * @param status
     * @param name
     */
    void recordExternal(bool status, QString name);

private:
    SwitchButton *recordToggle;
    QLineEdit *recordName;

    void setupUI();

private slots:

    void recordStateToggled(bool checked);
};

#endif  // RECORDAREA_H
