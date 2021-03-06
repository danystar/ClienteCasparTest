#include "AtemInputCommand.h"

#include <QtCore/QXmlStreamWriter>

AtemInputCommand::AtemInputCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& AtemInputCommand::getSwitcher() const
{
    return this->switcher;
}

const QString& AtemInputCommand::getInput() const
{
    return this->input;
}

bool AtemInputCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

const QString& AtemInputCommand::getMixerStep() const
{
    return this->mixerStep;
}

void AtemInputCommand::setSwitcher(const QString& switcher)
{
    this->switcher = switcher;
    emit switcherChanged(this->switcher);
}

void AtemInputCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
}

void AtemInputCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemInputCommand::setMixerStep(const QString& mixerStep)
{
    this->mixerStep = mixerStep;
    emit mixerStepChanged(this->mixerStep);
}

void AtemInputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSwitcher(QString::fromStdWString(pt.get(L"switcher", Atem::DEFAULT_SWITCHER.toStdWString())));
    setInput(QString::fromStdWString(pt.get(L"input", Atem::DEFAULT_INPUT.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
    setMixerStep(QString::fromStdWString(pt.get(L"mixerstep", Atem::DEFAULT_MIXER_STEP.toStdWString())));
}

void AtemInputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("switcher", this->getSwitcher());
    writer->writeTextElement("input", this->getInput());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("mixerstep", this->getMixerStep());
}
