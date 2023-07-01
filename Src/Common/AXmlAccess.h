/****************************************************************************
 * @file    AXmlAccess.h
 * @date    2023-07-01 
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 *
 * This source file is part of Aproch.
 * Copyright (C) 2020 by MonoKelvin. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#pragma once
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "AprochGlobal.h"

APROCH_NAMESPACE_BEGIN

#ifndef QT_NO_XMLSTREAMREADER
class IXmlStreamReader: public QObject
{
public:
    explicit IXmlStreamReader(QObject* parent) : QObject(parent) {}
    virtual ~IXmlStreamReader() {}
    virtual void setDevice(QIODevice* device) { Q_UNUSED(device); }
    virtual QIODevice* device() const { return nullptr; }
    virtual void clear() {}
    virtual bool atEnd() const { return true; }
    virtual QXmlStreamReader::TokenType readNext() {
        return QXmlStreamReader::NoToken;
    }
    virtual bool readNextStartElement() {
        return false;
    }
    virtual void skipCurrentElement() {}
    virtual QXmlStreamReader::TokenType tokenType() const {
        return QXmlStreamReader::NoToken;
    }
    virtual QString tokenString() const {
        return QString();
    }
    virtual void setNamespaceProcessing(bool) {}
    virtual bool namespaceProcessing() const { return false; }
    inline bool isStartDocument() const { return tokenType() == QXmlStreamReader::StartDocument; }
    inline bool isEndDocument() const { return tokenType() == QXmlStreamReader::EndDocument; }
    inline bool isStartElement() const { return tokenType() == QXmlStreamReader::StartElement; }
    inline bool isEndElement() const { return tokenType() == QXmlStreamReader::EndElement; }
    inline bool isCharacters() const { return tokenType() == QXmlStreamReader::Characters; }
    virtual bool isWhitespace() const { return false; }
    virtual bool isCDATA() const { return false; }
    inline bool isComment() const { return tokenType() == QXmlStreamReader::Comment; }
    inline bool isDTD() const { return tokenType() == QXmlStreamReader::DTD; }
    inline bool isEntityReference() const { return tokenType() == QXmlStreamReader::EntityReference; }
    inline bool isProcessingInstruction() const { return tokenType() == QXmlStreamReader::ProcessingInstruction; }
    virtual bool isStandaloneDocument() const { return false; }
    virtual QStringSubset documentVersion() const { return QStringSubset(); }
    virtual QStringSubset documentEncoding() const { return QStringSubset(); }
    virtual qint64 lineNumber() const { return 0; }
    virtual qint64 columnNumber() const { return 0; }
    virtual qint64 characterOffset() const { return 0; }
    virtual QXmlStreamAttributes attributes() const { return QXmlStreamAttributes(); }
#if 0
    virtual QString readElementText(QXmlStreamReader::ReadElementTextBehaviour behaviour = QXmlStreamReader::ErrorOnUnexpectedElement) { return QString(); }
#endif
    virtual QStringSubset name() const { return QStringSubset(); }
    virtual QStringSubset namespaceUri() const { return QStringSubset(); }
    virtual QStringSubset qualifiedName() const { return QStringSubset(); }
    virtual QStringSubset prefix() const { return QStringSubset(); }
    virtual QStringSubset processingInstructionTarget() const { return QStringSubset(); }
    virtual QStringSubset processingInstructionData() const { return QStringSubset(); }
    virtual QStringSubset text() const { return QStringSubset(); }
    virtual QXmlStreamNamespaceDeclarations namespaceDeclarations() const {
        return QXmlStreamNamespaceDeclarations();
    }
    virtual QXmlStreamNotationDeclarations notationDeclarations() const {
        return QXmlStreamNotationDeclarations();
    }
    virtual QXmlStreamEntityDeclarations entityDeclarations() const {
        return QXmlStreamEntityDeclarations();
    }
    virtual QStringSubset dtdName() const { return QStringSubset(); }
    virtual QStringSubset dtdPublicId() const { return QStringSubset(); }
    virtual QStringSubset dtdSystemId() const { return QStringSubset(); }
    virtual int entityExpansionLimit() const { return 0; }
    virtual void setEntityExpansionLimit(int limit) {
        Q_UNUSED(limit);
    }
    virtual QString errorString() const { return QString(); }
    virtual bool hasError() const { return true; }
};

class AXmlStreamReader: public IXmlStreamReader
{
public:
    explicit AXmlStreamReader(QObject* parent = nullptr) : IXmlStreamReader(parent), m_stream() {
    }
    explicit AXmlStreamReader(QIODevice* device, QObject* parent = nullptr) : IXmlStreamReader(parent), m_stream(device) {
    }
    void setDevice(QIODevice* device) override {
        m_stream.setDevice(device);
    }
    QIODevice* device() const override {
        return m_stream.device();
    }
    void clear() override {
        m_stream.clear();
    }
    bool atEnd() const override {
        return m_stream.atEnd();
    }
    QXmlStreamReader::TokenType readNext() override {
        return m_stream.readNext();
    }
    bool readNextStartElement() override {
        return m_stream.readNextStartElement();
    }
    void skipCurrentElement() override {
        m_stream.skipCurrentElement();
    }
    QXmlStreamReader::TokenType tokenType() const override {
        return m_stream.tokenType();
    }
    QString tokenString() const override {
        return m_stream.tokenString();
    }
    void setNamespaceProcessing(bool namespaceProcessing) override {
        m_stream.setNamespaceProcessing(namespaceProcessing);
    }
    bool namespaceProcessing() const override {
        return m_stream.namespaceProcessing();
    }
    bool isWhitespace() const override {
        return m_stream.isWhitespace();
    }
    bool isCDATA() const override {
        return m_stream.isCDATA();
    }
    bool isStandaloneDocument() const override {
        return m_stream.isStandaloneDocument();
    }
    QStringSubset documentVersion() const override {
        return m_stream.documentVersion();
    }
    QStringSubset documentEncoding() const override {
        return m_stream.documentEncoding();
    }
    qint64 lineNumber() const override {
        return m_stream.lineNumber();
    }
    qint64 columnNumber() const override {
        return m_stream.columnNumber();
    }
    qint64 characterOffset() const override {
        return m_stream.characterOffset();
    }
    QXmlStreamAttributes attributes() const override {
        return m_stream.attributes();
    }
#if 0
    QString readElementText(QXmlStreamReader::ReadElementTextBehaviour behaviour = QXmlStreamReader::ErrorOnUnexpectedElement) override {
        return m_stream.readElementText(behaviour);
    }
#endif
    QStringSubset name() const override {
        return m_stream.name();
    }
    QStringSubset namespaceUri() const override {
        return m_stream.namespaceUri();
    }
    QStringSubset qualifiedName() const override {
        return m_stream.qualifiedName();
    }
    QStringSubset prefix() const override {
        return m_stream.prefix();
    }
    QStringSubset processingInstructionTarget() const override {
        return m_stream.processingInstructionTarget();
    }
    QStringSubset processingInstructionData() const override {
        return m_stream.processingInstructionData();
    }
    QStringSubset text() const override {
        return m_stream.text();
    }
    QXmlStreamNamespaceDeclarations namespaceDeclarations() const override {
        return m_stream.namespaceDeclarations();
    }
    QXmlStreamNotationDeclarations notationDeclarations() const override {
        return m_stream.notationDeclarations();
    }
    QXmlStreamEntityDeclarations entityDeclarations() const override {
        return m_stream.entityDeclarations();
    }
    QStringSubset dtdName() const override {
        return m_stream.dtdName();
    }
    QStringSubset dtdPublicId() const override {
        return m_stream.dtdPublicId();
    }
    QStringSubset dtdSystemId() const override {
        return m_stream.dtdSystemId();
    }
    int entityExpansionLimit() const override {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        return m_stream.entityExpansionLimit();
#else
        return 0;
#endif
    }
    void setEntityExpansionLimit(int limit) override {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        m_stream.setEntityExpansionLimit(limit);
#else
        Q_UNUSED(limit);
#endif
    }
    virtual QString errorString() const override {
        return m_stream.errorString();
    }
    bool hasError() const override {
        return m_stream.hasError();
    }
private:
    QXmlStreamReader m_stream;
};

#endif //QT_NO_XMLSTREAMREADER

class IXmlStreamWriter : public QObject
{
public:
    IXmlStreamWriter(QObject* parent) : QObject(parent) {}
    virtual ~IXmlStreamWriter() {}
    virtual void setDevice(QIODevice* device) { Q_UNUSED(device); }
    virtual QIODevice* device() const { return nullptr; }
    virtual void setAutoFormatting(bool) {};
    virtual bool autoFormatting() const { return false;  }
    virtual void setAutoFormattingIndent(int spacesOrTabs) { Q_UNUSED(spacesOrTabs);  }
    virtual int autoFormattingIndent() const { return 0; }
    virtual void writeAttribute(const QString& qualifiedName, const QString& value) { Q_UNUSED(qualifiedName); Q_UNUSED(value); }
    virtual void writeAttribute(const QString& namespaceUri, const QString& name, const QString& value) { 
        Q_UNUSED(namespaceUri);
        Q_UNUSED(name);
        Q_UNUSED(value);
    }
    virtual void writeAttribute(const QXmlStreamAttribute& attribute) {
        Q_UNUSED(attribute);
    }
    virtual void writeAttributes(const QXmlStreamAttributes& attributes) {
        Q_UNUSED(attributes);
    }
    virtual void writeCDATA(const QString& text) { 
        Q_UNUSED(text);
    }
    virtual void writeCharacters(const QString& text) {
        Q_UNUSED(text);
    }
    virtual void writeComment(const QString& text) {
        Q_UNUSED(text);
    }
    virtual void writeDTD(const QString& name, const QString& systemId, const QString& publicId) {
        Q_UNUSED(name);
        Q_UNUSED(systemId);
        Q_UNUSED(publicId);
    }
    virtual void writeEmptyElement(const QString& qualifiedName) {
        Q_UNUSED(qualifiedName);
    }
    virtual void writeEmptyElement(const QString& namespaceUri, const QString& name) {
        Q_UNUSED(namespaceUri);
        Q_UNUSED(name);
    }
    virtual void writeTextElement(const QString& qualifiedName, const QString& text) {
        Q_UNUSED(qualifiedName);
        Q_UNUSED(text);
    }
    virtual void writeTextElement(const QString& namespaceUri, const QString& name, const QString& text) {
        Q_UNUSED(namespaceUri);
        Q_UNUSED(name);
        Q_UNUSED(text);
    }
    virtual void writeEndDocument() {}
    virtual void writeEndElement() {}
    virtual void writeEntityReference(const QString& name) {
        Q_UNUSED(name);
    }
    virtual void writeNamespace(const QString& namespaceUri, const QString& prefix = QString()) {
        Q_UNUSED(namespaceUri);
        Q_UNUSED(prefix);
    }
    virtual void writeDefaultNamespace(const QString& namespaceUri) {
        Q_UNUSED(namespaceUri);
    }
    virtual void writeProcessingInstruction(const QString& target, const QString& data = QString()) {
        Q_UNUSED(target);
        Q_UNUSED(data);
    }
    virtual void writeStartDocument() {}
    virtual void writeStartDocument(const QString& version) {
        Q_UNUSED(version);
    }
    virtual void writeStartDocument(const QString& version, bool standalone) {
        Q_UNUSED(version);
        Q_UNUSED(standalone);
    }
    virtual void writeStartElement(const QString& qualifiedName) {
        Q_UNUSED(qualifiedName);
    }
    virtual void writeStartElement(const QString& namespaceUri, const QString& name) {
        Q_UNUSED(namespaceUri);
        Q_UNUSED(name);
    }
#ifndef QT_NO_XMLSTREAMREADER
    virtual void writeCurrentToken(const QXmlStreamReader& reader) {
        Q_UNUSED(reader);
    }
#endif
    virtual bool hasError() const { return true; }
};

class AXmlStreamWriter: public IXmlStreamWriter
{
public:
    AXmlStreamWriter(QObject* parent = nullptr) : IXmlStreamWriter(parent), m_stream() {
    }
    explicit AXmlStreamWriter(QIODevice* device, QObject* parent = nullptr): IXmlStreamWriter(parent), m_stream(device) {
    }
    void setDevice(QIODevice* device) override {
        m_stream.setDevice(device);
    }
    QIODevice* device() const override {
        return m_stream.device();
    }
    void setAutoFormatting(bool autoFormatting) override {
        m_stream.setAutoFormatting(autoFormatting);
    }
    bool autoFormatting() const override {
        return m_stream.autoFormatting();
    }
    void setAutoFormattingIndent(int spacesOrTabs) override {
        m_stream.setAutoFormattingIndent(spacesOrTabs);
    }
    int autoFormattingIndent() const override {
        return m_stream.autoFormattingIndent();
    }
    void writeAttribute(const QString& qualifiedName, const QString& value) override {
        m_stream.writeAttribute(qualifiedName, value);
    }
    void writeAttribute(const QString& namespaceUri, const QString& name, const QString& value) override {
        m_stream.writeAttribute(namespaceUri, name, value);
    }
    void writeAttribute(const QXmlStreamAttribute& attribute) override {
        m_stream.writeAttribute(attribute);
    }
    void writeAttributes(const QXmlStreamAttributes& attributes) override {
        m_stream.writeAttributes(attributes);
    }
    void writeCDATA(const QString& text) override {
        m_stream.writeCDATA(text);
    }
    void writeCharacters(const QString& text) override {
        m_stream.writeCharacters(text);
    }
    void writeComment(const QString& text) override {
        m_stream.writeComment(text);
    }
    void writeDTD(const QString& name, const QString& systemId, const QString& publicId) override {
        QString dtd;
        if (!name.isEmpty())
            dtd += QLatin1String("<!DOCTYPE ") + name;
        else
            dtd += QLatin1String("<!DOCTYPE dtdname");

        if (!systemId.isEmpty())
            dtd += QString(QLatin1String(" SYSTEM \"%1\"")).arg(systemId);
        if (!publicId.isEmpty())
            dtd += QString(QLatin1String(" PUBLIC \"%1\"")).arg(publicId);
        dtd += QLatin1String(">");
        m_stream.writeDTD(dtd);
    }
    void writeEmptyElement(const QString& qualifiedName) override {
        m_stream.writeEmptyElement(qualifiedName);
    }
    void writeEmptyElement(const QString& namespaceUri, const QString& name) override {
        m_stream.writeEmptyElement(namespaceUri, name);
    }
    void writeTextElement(const QString& qualifiedName, const QString& text) override {
        m_stream.writeTextElement(qualifiedName, text);
    }
    void writeTextElement(const QString& namespaceUri, const QString& name, const QString& text) override {
        m_stream.writeTextElement(namespaceUri, name, text);
    }
    void writeEndDocument() override {
        m_stream.writeEndDocument();
    }
    void writeEndElement() override {
        m_stream.writeEndElement();
    }
    void writeEntityReference(const QString& name) override {
        m_stream.writeEntityReference(name);
    }
    void writeNamespace(const QString& namespaceUri, const QString& prefix = QString()) override {
        m_stream.writeNamespace(namespaceUri, prefix);
    }
    void writeDefaultNamespace(const QString& namespaceUri) override {
        m_stream.writeDefaultNamespace(namespaceUri);
    }
    void writeProcessingInstruction(const QString& target, const QString& data = QString()) override {
        m_stream.writeProcessingInstruction(target, data);
    }
    void writeStartDocument() override {
        m_stream.writeStartDocument();
    }
    void writeStartDocument(const QString& version) override {
        m_stream.writeStartDocument(version);
    }
    void writeStartDocument(const QString& version, bool standalone) override {
        m_stream.writeStartDocument(version, standalone);
    }
    void writeStartElement(const QString& qualifiedName) override {
        m_stream.writeStartElement(qualifiedName);
    }
    void writeStartElement(const QString& namespaceUri, const QString& name) override {
        m_stream.writeStartElement(namespaceUri, name);
    }
#ifndef QT_NO_XMLSTREAMREADER
    void writeCurrentToken(const QXmlStreamReader& reader) override {
        m_stream.writeCurrentToken(reader);
    }
#endif
    bool hasError() const override {
        return m_stream.hasError();
    }
private:
    QXmlStreamWriter m_stream;
};

APROCH_NAMESPACE_END