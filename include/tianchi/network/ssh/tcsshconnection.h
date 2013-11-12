/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/

//Qt ssh连接类
#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include "tcssherrors.h"

#include <tianchi/tcglobal.h>

#include <QByteArray>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QHostAddress>

namespace TCssh {
class SftpChannel;
class SshRemoteProcess;

namespace Internal {
class SshConnectionPrivate;
} // namespace Internal

class TIANCHI_API SshConnectionParameters//ssh连接参数类
{
public:
    enum ProxyType { DefaultProxy, NoProxy };//代理模式
    enum AuthenticationType { AuthenticationByPassword, AuthenticationByKey };//认证模式（密码认证，密匙认证）
    SshConnectionParameters();

    QString host;//主机
    QString userName;//用户名
    QString password;//密码
    QString privateKeyFile;//密匙文件（应该是地址）
    int timeout; // In seconds.//超时？单位为秒
    AuthenticationType authenticationType; //连接模式
    quint16 port; //端口
    ProxyType proxyType;//代理
};

//重载==和！==运算符
QSSH_EXPORT bool operator==(const SshConnectionParameters &p1, const SshConnectionParameters &p2);
QSSH_EXPORT bool operator!=(const SshConnectionParameters &p1, const SshConnectionParameters &p2);

class TIANCHI_API SshConnectionInfo //连接信息
{
public:
    SshConnectionInfo() : localPort(0), peerPort(0) {}
    SshConnectionInfo(const QHostAddress &la, quint16 lp, const QHostAddress &pa, quint16 pp)
        : localAddress(la), localPort(lp), peerAddress(pa), peerPort(pp) {}

    QHostAddress localAddress;//本机IP？
    quint16 localPort;
    QHostAddress peerAddress;
    quint16 peerPort;
};

class TIANCHI_API SshConnection : public QObject //ssh连接
{
    Q_OBJECT

public:
    enum State { Unconnected, Connecting, Connected };
    //连接状态枚举｛未连接，连接中，已连接｝

    explicit SshConnection(const SshConnectionParameters &serverInfo, QObject *parent = 0);
    //构造函数，参数为连接信息类

    void connectToHost();
    void disconnectFromHost();
    State state() const;//连接状态
    SshError errorState() const;//返回错误信息
    QString errorString() const;//字符串方式返回错误信息
    SshConnectionParameters connectionParameters() const;//得到连接方式信息
    SshConnectionInfo connectionInfo() const;//得到连接信息
    ~SshConnection();

    QSharedPointer<SshRemoteProcess> createRemoteProcess(const QByteArray &command);
    QSharedPointer<SshRemoteProcess> createRemoteShell();
    QSharedPointer<SftpChannel> createSftpChannel();

    // -1 if an error occurred, number of channels closed otherwise.
    int closeAllChannels();

    int channelCount() const;

signals: //发出的信号
    void connected();//已连接
    void disconnected();//断开连接
    void dataAvailable(const QString &message);
    void error(QSsh::SshError);//错误？

private:
    Internal::SshConnectionPrivate *d;
};

} // namespace TCssh

#endif // SSHCONNECTION_H
