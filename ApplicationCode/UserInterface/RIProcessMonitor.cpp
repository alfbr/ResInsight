/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RIStdInclude.h"

#include "RIProcessMonitor.h"
#include "cafUiProcess.h"
#include "RIApplication.h"


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RIProcessMonitor::RIProcessMonitor(QDockWidget* pParent)
    : QWidget(pParent)
{
    m_monitoredProcess = NULL;

    QLabel* pLabel = new QLabel("Status:", this);
    pLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_labelStatus = new QLabel(this);

    QHBoxLayout* pTopLayout = new QHBoxLayout;
    pTopLayout->addWidget(pLabel);
    pTopLayout->addWidget(m_labelStatus);

    pTopLayout->addStretch();
    QPushButton* clearPushButton = new QPushButton("Clear", this);
    pTopLayout->addWidget(clearPushButton);
    connect(clearPushButton, SIGNAL(clicked()), this, SLOT(slotClearTextEdit()) );

    m_terminatePushButton = new QPushButton("Stop", this);
    pTopLayout->addWidget(m_terminatePushButton);
    connect(m_terminatePushButton, SIGNAL(clicked()), this, SLOT(slotTerminateProcess()) );
    m_terminatePushButton->setEnabled(false);

    m_textEdit = new QPlainTextEdit(this);
    m_textEdit->setReadOnly(true);
    m_textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

    //QFont font("Courier", 8);
    QFont font("Terminal", 10);
    m_textEdit->setFont(font);

    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->addLayout(pTopLayout);
    pLayout->addWidget(m_textEdit);

    setLayout(pLayout);

    setStatusMsg("N/A", caf::PROCESS_STATE_NORMAL);
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RIProcessMonitor::~RIProcessMonitor()
{

}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::startMonitorWorkProcess(caf::UiProcess* pProcess)
{
    setStatusMsg("N/A", caf::PROCESS_STATE_NORMAL);

    if (m_monitoredProcess == pProcess) return;

    m_monitoredProcess = pProcess;
    if (!m_monitoredProcess) return;

    connect(m_monitoredProcess, SIGNAL(signalStatusMsg(const QString&, int)),    SLOT(slotShowProcStatusMsg(const QString&, int)));
    connect(m_monitoredProcess, SIGNAL(readyReadStandardError()),                SLOT(slotProcReadyReadStdErr()));
    connect(m_monitoredProcess, SIGNAL(readyReadStandardOutput()),                SLOT(slotProcReadyReadStdOut()));

    m_terminatePushButton->setEnabled(true);

    QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
    addStringToLog(timeStamp + " Process starting\n");
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::stopMonitorWorkProcess()
{
    m_monitoredProcess = NULL;

    m_terminatePushButton->setEnabled(false);

    setStatusMsg("N/A", caf::PROCESS_STATE_NORMAL);

    QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
    addStringToLog(timeStamp + " Process finished\n\n");
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::setStatusMsg(const QString& sStatusMsg, int iStatusMsgType)
{
    if (!m_labelStatus) return;

    QString sMsg;

    switch (iStatusMsgType)
    {
        case caf::PROCESS_STATE_RUNNING:    sMsg = "<font color='green'>"   + sStatusMsg + "</font>";   break;
        case caf::PROCESS_STATE_ERROR:      sMsg = "<font color='red'>"     + sStatusMsg + "</font>";   break;
        default:                            sMsg = sStatusMsg;
    }

    m_labelStatus->setText(sMsg);
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::addStringToLog(const QString& sTxt)
{
    m_textEdit->moveCursor(QTextCursor::End);
    m_textEdit->insertPlainText(sTxt);

    m_textEdit->ensureCursorVisible();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::slotShowProcStatusMsg(const QString& sMsg, int iStatusMsgType)
{
    setStatusMsg(sMsg, iStatusMsgType);
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::slotProcReadyReadStdOut()
{
    if (!m_monitoredProcess) return;

    QByteArray dataArray = m_monitoredProcess->readAllStandardOutput();

    dataArray.replace("\r", "");

    addStringToLog(dataArray.data());
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::slotProcReadyReadStdErr()
{
    if (!m_monitoredProcess) return;

    QByteArray dataArray = m_monitoredProcess->readAllStandardError();

    dataArray.replace("\r", "");

    addStringToLog(dataArray.data());
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::slotTerminateProcess()
{
    addStringToLog("Process terminated by user\n");

    RIApplication* app = RIApplication::instance();
    app->terminateProcess();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RIProcessMonitor::slotClearTextEdit()
{
    m_textEdit->clear();
}

