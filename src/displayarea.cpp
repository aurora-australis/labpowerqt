// This file is part of labpowerqt, a Gui application to control programmable
// lab power supplies.
// Copyright © 2015 Christian Rapp <0x2a at posteo dot org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "displayarea.h"

namespace globcon = global_constants;
namespace setcon = settings_constants;

DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent)
{
    this->setupUI();
    this->setupChannels();
}

void DisplayArea::setValuesDialog(std::shared_ptr<FloatingValuesDialogData> valuesDialogData, std::shared_ptr<FloatingValuesDialog> valuesDialog)
{
    this->valuesDialog = valuesDialog;
    this->valuesDialogData = valuesDialogData;
}

void DisplayArea::setupChannels()
{
    QSettings settings;
    settings.beginGroup(setcon::DEVICE_GROUP);
    if (settings.contains(setcon::DEVICE_PORT)) {
        // TODO: One could make this more intelligent and only create / delete
        // channels that are (no longer) needed.

        // remove all channels from the frame
        for (auto f : this->channelFramesVec) {
            this->frameChannels->layout()->removeWidget(f);
            delete (f);
        }
        // clear the vector
        this->channelFramesVec.clear();
        this->chanwVector.clear();

        // create the frames that control one channel
        for (int i = 1; i <= settings.value(setcon::DEVICE_CHANNELS).toInt();
             i++) {

            std::unique_ptr<ChannelWidgets> chanw(new ChannelWidgets());

            QFrame *channelFrameContainer = new QFrame();
            channelFrameContainer->setLayout(new QVBoxLayout());
            channelFrameContainer->setObjectName("ch" + QString::number(i) +
                                                 "Container");
            // this is the container frame
            QString frameQss = QString("QFrame#%1 {border: 1px solid ") +
                               globcon::GREENCOLOR + ";}";
            channelFrameContainer->setStyleSheet(
                frameQss.arg(channelFrameContainer->objectName()));
            channelFrameContainer->layout()->setSpacing(0);
            channelFrameContainer->layout()->setContentsMargins(QMargins());

            this->frameChannels->layout()->addWidget(channelFrameContainer);

            // now the frame for the electrical values V, A, W.

            QFrame *vawFrame = new QFrame();
            vawFrame->setObjectName("ch" + QString::number(i) + "vawFrame");
            QString vawFrameQss =
                QString("QFrame#%1 {border-bottom: 1px solid ") +
                globcon::GREENCOLOR + ";}";
            vawFrame->setStyleSheet(vawFrameQss.arg(vawFrame->objectName()));
            QGridLayout *vawLayout = new QGridLayout();
            vawFrame->setLayout(vawLayout);
            channelFrameContainer->layout()->addWidget(vawFrame);

            vawLayout->addWidget(new QLabel("CH" + QString::number(i)), 0, 0, 1,
                                 0, Qt::AlignLeft);

            chanw->voltageActual = new QLabel("0.00");
            QFont actualFont = chanw->voltageActual->font();
            actualFont.setPointSize(20);
            chanw->voltageActual->setAlignment(Qt::AlignRight |
                                               Qt::AlignVCenter);
            chanw->voltageActual->setStyleSheet(
                QString("QLabel {padding-right: 0.5em;}"));
            chanw->voltageActual->setFont(actualFont);
            chanw->currentActual = new QLabel("0.000");
            chanw->currentActual->setAlignment(Qt::AlignRight |
                                               Qt::AlignVCenter);
            chanw->currentActual->setStyleSheet(
                QString("QLabel {padding-right: 0.5em;}"));
            chanw->currentActual->setFont(actualFont);
            chanw->wattageActual = new QLabel("0.000");
            chanw->wattageActual->setAlignment(Qt::AlignRight |
                                               Qt::AlignVCenter);
            chanw->wattageActual->setStyleSheet(
                QString("QLabel {padding-right: 0.5em;}"));
            chanw->wattageActual->setFont(actualFont);
            vawLayout->addWidget(chanw->voltageActual, 1, 0);
            vawLayout->addWidget(chanw->currentActual, 2, 0);
            vawLayout->addWidget(chanw->wattageActual, 3, 0);

            QLabel *vActualUnit = new QLabel("V");
            vActualUnit->setSizePolicy(QSizePolicy::Policy::Fixed,
                                       QSizePolicy::Policy::Preferred);
            vActualUnit->setAlignment(Qt::AlignTop);
            QLabel *aActualUnit = new QLabel("A");
            aActualUnit->setSizePolicy(QSizePolicy::Policy::Fixed,
                                       QSizePolicy::Policy::Preferred);
            aActualUnit->setAlignment(Qt::AlignTop);
            QLabel *wActualUnit = new QLabel("W");
            wActualUnit->setSizePolicy(QSizePolicy::Policy::Fixed,
                                       QSizePolicy::Policy::Preferred);
            wActualUnit->setAlignment(Qt::AlignTop);
            vawLayout->addWidget(vActualUnit, 1, 1);
            vawLayout->addWidget(aActualUnit, 2, 1);
            vawLayout->addWidget(wActualUnit, 3, 1);

            QFrame *bottomContainer = new QFrame();
            bottomContainer->setLayout(new QHBoxLayout());
            channelFrameContainer->layout()->addWidget(bottomContainer);
            bottomContainer->layout()->setSpacing(0);
            bottomContainer->layout()->setContentsMargins(QMargins());

            QFrame *setContainer = new QFrame();
            setContainer->setObjectName("ch" + QString::number(i) +
                                        "setContainer");
            QGridLayout *setContLayout = new QGridLayout();
            setContainer->setLayout(setContLayout);
            QString setContainerQss =
                QString("QFrame#%1 {border-right: 1px solid ") +
                globcon::GREENCOLOR + ";}";
            setContainer->setStyleSheet(
                setContainerQss.arg(setContainer->objectName()));
            chanw->voltageSet = new ClickableLabel("0.00");
            chanw->currentSet = new ClickableLabel("0.000");
            chanw->voltageSet->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            chanw->currentSet->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            QLabel *setLabel = new QLabel("Set");
            setLabel->setAlignment(Qt::AlignCenter);
            QLabel *voltageSetUnit = new QLabel("V");
            QFont bottomUnitFont = voltageSetUnit->font();
            bottomUnitFont.setPointSize(8);
            voltageSetUnit->setFont(bottomUnitFont);
            voltageSetUnit->setSizePolicy(QSizePolicy::Policy::Fixed,
                                          QSizePolicy::Policy::Preferred);
            voltageSetUnit->setAlignment(Qt::AlignTop);
            QLabel *currentSetUnit = new QLabel("A");
            currentSetUnit->setFont(bottomUnitFont);
            currentSetUnit->setSizePolicy(QSizePolicy::Policy::Fixed,
                                          QSizePolicy::Policy::Preferred);
            currentSetUnit->setAlignment(Qt::AlignTop);
            setContLayout->addWidget(setLabel, 0, 0, 1, 0);
            setContLayout->addWidget(chanw->voltageSet, 1, 0);
            setContLayout->addWidget(voltageSetUnit, 1, 1);
            setContLayout->addWidget(chanw->currentSet, 2, 0);
            setContLayout->addWidget(currentSetUnit, 2, 1);

            QFrame *modeOutputContainer = new QFrame();
            modeOutputContainer->setLayout(new QVBoxLayout());
            modeOutputContainer->layout()->setSpacing(0);
            modeOutputContainer->layout()->setContentsMargins(QMargins());

            QFrame *modeContainer = new QFrame();
            modeContainer->setLayout(new QHBoxLayout());
            modeContainer->setObjectName("ch" + QString::number(i) +
                                         "modeContainer");
            QString modeContainerQss =
                QString("QFrame#%1 {border-bottom: 1px solid ") +
                globcon::GREENCOLOR + ";}";
            modeContainer->setStyleSheet(
                modeContainerQss.arg(modeContainer->objectName()));
            QLabel *modeLabel = new QLabel("Mode");
            chanw->modeActual = new QLabel("CV");
            modeContainer->layout()->addWidget(modeLabel);
            modeContainer->layout()->addWidget(chanw->modeActual);

            QFrame *outputContainer = new QFrame();
            outputContainer->setLayout(new QHBoxLayout());
            QLabel *outputLabel = new QLabel("Output");
            chanw->outputSet = new ClickableLabel("Off");
            outputContainer->layout()->addWidget(outputLabel);
            outputContainer->layout()->addWidget(chanw->outputSet);

            modeOutputContainer->layout()->addWidget(modeContainer);
            modeOutputContainer->layout()->addWidget(outputContainer);

            bottomContainer->layout()->addWidget(setContainer);
            bottomContainer->layout()->addWidget(modeOutputContainer);

            this->channelFramesVec.push_back(channelFrameContainer);
            // move semantics are needed here because unique_ptr does not allow
            // copying what push_back actually does.
            this->chanwVector.push_back(std::move(chanw));
        }
    }
}

void DisplayArea::setupUI()
{

    this->setLayout(new QGridLayout());
    this->setStyleSheet("background-color: rgb(47, 47, 47); color: " +
                        QString(globcon::GREENCOLOR) + ";");
    this->setAutoFillBackground(true);
    this->layout()->setSpacing(0);
    QMargins layMargins = this->layout()->contentsMargins();
    layMargins.setBottom(0);
    layMargins.setTop(0);
    this->layout()->setContentsMargins(layMargins);

    // init header
    this->frameHeader = new QFrame();
    this->frameHeader->setLayout(new QHBoxLayout());
    dynamic_cast<QGridLayout *>(this->layout())
        ->addWidget(this->frameHeader, 0, 0);

    this->labelDeviceName = new QLabel();
    this->labelDeviceName->setText("Unknown Device");
    this->frameHeader->layout()->addWidget(this->labelDeviceName);

    dynamic_cast<QHBoxLayout *>(this->frameHeader->layout())->addStretch();

    this->labelConnect = new ClickableLabel();
    this->labelConnect->setPixmap(QPixmap(":/icons/plug_out_orange.png"));
    this->labelConnect->setMinimumWidth(37);
    this->labelConnect->setToolTip("Disconnected");
    this->frameHeader->layout()->addWidget(this->labelConnect);

    this->labelSound = new ClickableLabel();
    this->labelSound->setPixmap(QPixmap(":/icons/speaker_orange.png"));
    this->labelSound->setMinimumWidth(25);
    this->labelSound->setToolTip("Sound on");
    this->frameHeader->layout()->addWidget(this->labelSound);

    this->labelConnect = new ClickableLabel();
    this->labelConnect->setPixmap(QPixmap(":/icons/lock-open_orange.png"));
    this->labelConnect->setMinimumSize(QSize(25, 16));
    this->labelConnect->setToolTip("Device Controls unlocked");
    this->frameHeader->layout()->addWidget(this->labelConnect);

    this->headerControls = {this->labelConnect, this->labelSound};

    // init channels
    this->frameChannels = new QFrame();
    this->frameChannels->setLayout(new QHBoxLayout());
    this->frameChannels->layout()->setSpacing(0);
    this->frameChannels->layout()->setContentsMargins(QMargins());
    dynamic_cast<QGridLayout *>(this->layout())
        ->addWidget(this->frameChannels, 1, 0);

    // init footer
    this->frameFooter = new QFrame();
    this->frameFooter->setLayout(new QHBoxLayout());
    this->frameFooter->layout()->setContentsMargins(0, 0, 0, 0);
    this->frameFooter->layout()->setSpacing(0);

    dynamic_cast<QGridLayout *>(this->layout())
        ->addWidget(this->frameFooter, 2, 0);

    QFrame *frameOVP = new QFrame();
    frameOVP->setLayout(new QHBoxLayout());
    frameOVP->layout()->addWidget(new QLabel("OVP:"));
    this->labelOVPSet = new ClickableLabel();
    this->labelOVPSet->setText("Off");
    frameOVP->layout()->addWidget(this->labelOVPSet);
    this->frameFooter->layout()->addWidget(frameOVP);

    QFrame *frameOCP = new QFrame();
    frameOCP->setLayout(new QHBoxLayout());
    frameOCP->layout()->addWidget(new QLabel("OCP:"));
    this->labelOCPSet = new ClickableLabel();
    this->labelOCPSet->setText("Off");
    frameOCP->layout()->addWidget(this->labelOCPSet);
    this->frameFooter->layout()->addWidget(frameOCP);

    QFrame *frameOTP = new QFrame();
    frameOTP->setLayout(new QHBoxLayout());
    frameOTP->layout()->addWidget(new QLabel("OTP:"));
    this->labelOTPSet = new ClickableLabel();
    this->labelOTPSet->setText("Off");
    frameOTP->layout()->addWidget(this->labelOTPSet);
    this->frameFooter->layout()->addWidget(frameOTP);

    QFrame *frameTracking = new QFrame();
    frameTracking->setLayout(new QHBoxLayout());
    frameTracking->layout()->addWidget(new QLabel("Tracking:"));
    this->labelTrackingSet = new ClickableLabel();
    this->labelTrackingSet->setText("None");
    frameTracking->layout()->addWidget(this->labelTrackingSet);
    this->frameFooter->layout()->addWidget(frameTracking);

    dynamic_cast<QHBoxLayout *>(frameTracking->layout())->addStretch();
}

void DisplayArea::paintEvent(QPaintEvent *event)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}