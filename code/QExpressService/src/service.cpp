/*
 * Copyright (c) 2013-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "service.hpp"

#include <bb/Application>
#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/system/InvokeManager>

#include <QTimer>

using namespace bb::platform;
using namespace bb::system;

Service::Service() :
        QObject(),
        m_notify(new Notification(this)),
        m_invokeManager(new InvokeManager(this))
{
    m_invokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));

    NotificationDefaultApplicationSettings settings;
    settings.setPreview(NotificationPriorityPolicy::Allow);
    settings.apply();

    m_notify->setTitle("QExpress Service");
    m_notify->setBody("QExpress service requires attention");

    bb::system::InvokeRequest request;
    request.setTarget("com.example.QExpress");
    request.setAction("bb.action.START");
    m_notify->setInvokeRequest(request);

    onTimeout();
}

void Service::handleInvoke(const bb::system::InvokeRequest & request)
{
    if (request.action().compare("com.example.QExpressService.RESET") == 0) {
        triggerNotification();
    }
}

void Service::triggerNotification()
{
    // Timeout is to give time for UI to minimize
    QTimer::singleShot(2000, this, SLOT(onTimeout()));
}

void Service::onTimeout()
{
    Notification::clearEffectsForAll();
    Notification::deleteAllFromInbox();
    m_notify->notify();
}
