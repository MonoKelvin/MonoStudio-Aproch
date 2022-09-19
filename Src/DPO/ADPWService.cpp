#include "stdafx.h"
#include "ADPWService.h"
#include <QtXml/QDomDocument>

namespace aproch
{
    const QString ADPWData::key = "key";
    QString ADPWService::nameSplitterChar = '|';

    QString ADPWData::value(const QString& attrName) const
    {
        const auto& attrs = domElement.attributes();
        for (int i = 0; i < attrs.size(); ++i)
        {
            const auto& attr = attrs.item(i);
            if (0 == attr.nodeName().compare(attrName, Qt::CaseInsensitive))
                return attr.nodeValue();
        }
        return ANull_String;
    }

    QMap<QString, QString> ADPWData::attributes(bool normalize) const
    {
        QMap<QString, QString> attrMap;
        const auto& attrs = domElement.attributes();
        for (int i = 0; i < attrs.size(); ++i)
        {
            const auto& attr = attrs.item(i);
            attrMap.insert(normalize ? attr.nodeName().trimmed().toLower() : attr.nodeName(), attr.nodeValue());
        }

        return attrMap;
    }

    ADPWService::~ADPWService()
    {
    }

    bool ADPWService::registerObjectCreator(const QSharedPointer<aproch::IObjectCreator>& creator)
    {
        if (nullptr == creator)
            return false;

        const auto& createName = creator->name();
        const QStringList keyList = createName.split(nameSplitterChar, QString::SkipEmptyParts, Qt::CaseInsensitive);
        for (const QString& key : keyList)
        {
            const QString newkey = _key(key);
            if (getObjectCreator(newkey))
            {
                // TODO:
                qWarning(QString("the [IObjectCreator] is exsits: %1").arg(key).toLatin1());
            }

            mCreatorMap[newkey] = creator;
        }

        return keyList.size() > 0;
    }

    bool ADPWService::unregisterObjectCreator(const QString& name)
    {
        auto iter = mCreatorMap.find(_key(name));
        if (iter == mCreatorMap.end())
            return false;
        mCreatorMap.erase(iter);
        return true;
    }

    QSharedPointer<aproch::IObjectCreator> ADPWService::getObjectCreator(const QString& name) const
    {
        auto iter = mCreatorMap.find(_key(name));
        if (iter == mCreatorMap.end())
            return nullptr;
        return iter.value();
    }

    QObject* ADPWService::prase(const QString& data, QObject* parent)
    {
        QDomDocument docXML;
        QString errStr;
        int row, column;
        if (!docXML.setContent(data, &errStr, &row, &column))
        {
            // TODO: log
            qWarning(QString("QDomDocument::setContent failed at [%1, %2]. Detail: %3")
                        .arg(row).arg(column).arg(errStr).toLatin1());
            return nullptr;
        }

        const QDomElement xmlRoot = docXML.documentElement();
        QObject* theObj = createObjectByDomElement(xmlRoot, parent);

        if (nullptr != theObj && nullptr != parent)
            theObj->setParent(parent);

        return theObj;
    }

    QString ADPWService::serialize(QObject* object)
    {
        return QString();
    }

    QObject* ADPWService::createObjectByDomElement(const QDomElement& domElement, QObject* parent)
    {
        const QString& objName = _key(domElement.tagName());
        QSharedPointer<aproch::IObjectCreator> pObjCreator = getObjectCreator(objName);
        if (nullptr == pObjCreator)
        {
            // TODO: log
            qWarning(QString("the object creator named \"%1\" is null").arg(domElement.tagName()).toLatin1());
            return nullptr;
        }

        ADPWData data;
        data.domElement = domElement;
        QObject* newObj = pObjCreator->createObject(objName, data, parent);
        if (nullptr == newObj)
        {
            // TODO: log
            qWarning(QString("the object named \"%1\" failed to create. line number:%2")
                        .arg(objName).arg(domElement.lineNumber()).toLatin1());
            return nullptr;
        }

        const auto& attrs = domElement.attributes();
        for (int i = 0; i < attrs.size(); ++i)
        {
            const auto& attr = attrs.item(i);

            // set key
            if (0 == attr.nodeName().compare(ADPWData::key, Qt::CaseInsensitive) && !attr.nodeValue().isEmpty())
                newObj->setProperty(ADPWData::key.toLatin1(), attr.nodeValue());

            // set property
            bool hasProp = false;
            QString propName = attr.nodeName().trimmed();
            if (propName.isEmpty())
                continue;
            if (newObj->property(propName.toLatin1()).isValid())
                hasProp = true;
            else
            {
                propName[0] = QChar(propName[0]).toLower();
                if (newObj->property(propName.toLatin1()).isValid())
                    hasProp = true;
            }

            if (hasProp)
                newObj->setProperty(propName.toLatin1(), attr.nodeValue());
        }

        QLayout* pLayout = qobject_cast<QLayout*>(newObj);
        QWidget* pWidget = qobject_cast<QWidget*>(newObj);
        if (nullptr == pLayout && nullptr == pWidget)
        {
            newObj->setParent(nullptr);
            newObj->deleteLater();
            return nullptr;
        }

        const QDomNodeList childrenNodes = domElement.childNodes();
        for (int i = 0; i < childrenNodes.size(); ++i)
        {
            if (!childrenNodes.at(i).isElement())
                continue;
            const QDomElement childDomElement = childrenNodes.at(i).toElement();
            QObject* childObj = createObjectByDomElement(childDomElement, parent);
            if (nullptr == childObj)
                continue;

            QLayout* childLayout = qobject_cast<QLayout*>(childObj);
            QWidget* childWidget = qobject_cast<QWidget*>(childObj);

            if (nullptr != pLayout)
            {
                if (nullptr != childLayout)
                {
                    pLayout->addItem(childLayout);
                }
                else if (nullptr != childWidget)
                {
                    pLayout->addWidget(childWidget);
                }
            }
            else if (nullptr != pWidget)
            {
                if (nullptr != childLayout)
                {
                    pWidget->setLayout(childLayout);
                }
                else if (nullptr != childWidget)
                {
                    childWidget->setParent(pWidget);
                }
            }
        }

        return newObj;
    }

    const QString ADPWService::_key(const QString& str) const
    {
        return str.trimmed().toLower();
    }
}
