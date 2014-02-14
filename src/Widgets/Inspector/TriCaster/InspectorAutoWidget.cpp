#include "InspectorAutoWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/TriCaster/TriCasterStepModel.h"
#include "Models/TriCaster/TriCasterAutoSpeedModel.h"
#include "Models/TriCaster/TriCasterAutoTransitionModel.h"

#include <QtGui/QApplication>

InspectorAutoWidget::InspectorAutoWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTriCasterStep();
    loadTriCasterAutoSpeed();
    loadTriCasterAutoTransition();

    qApp->installEventFilter(this);
}

bool InspectorAutoWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<AutoCommand*>(rundownItemSelectedEvent->getCommand()))
        {  
            this->command = dynamic_cast<AutoCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxStep->setCurrentIndex(this->comboBoxStep->findData(this->command->getStep()));
        }

        checkEmptyStep();

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorAutoWidget::blockAllSignals(bool block)
{
    this->comboBoxStep->blockSignals(block);
}

void InspectorAutoWidget::loadTriCasterStep()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxStep->blockSignals(true);

    QList<TriCasterStepModel> models = DatabaseManager::getInstance().getTriCasterStep();
    foreach (TriCasterStepModel model, models)
        this->comboBoxStep->addItem(model.getName(), model.getValue());

    this->comboBoxStep->blockSignals(false);
}

void InspectorAutoWidget::loadTriCasterAutoSpeed()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxSpeed->blockSignals(true);

    QList<TriCasterAutoSpeedModel> models = DatabaseManager::getInstance().getTriCasterAutoSpeed();
    foreach (TriCasterAutoSpeedModel model, models)
        this->comboBoxSpeed->addItem(model.getName(), model.getValue());

    this->comboBoxSpeed->blockSignals(false);
}

void InspectorAutoWidget::loadTriCasterAutoTransition()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTransition->blockSignals(true);

    QList<TriCasterAutoTransitionModel> models = DatabaseManager::getInstance().getTriCasterAutoTransition();
    foreach (TriCasterAutoTransitionModel model, models)
        this->comboBoxTransition->addItem(model.getName(), model.getValue());

    this->comboBoxTransition->blockSignals(false);
}

void InspectorAutoWidget::checkEmptyStep()
{
    if (this->comboBoxStep->isEnabled() && this->comboBoxStep->currentText() == "")
        this->comboBoxStep->setStyleSheet("border-color: firebrick;");
    else
        this->comboBoxStep->setStyleSheet("");
}

void InspectorAutoWidget::stepChanged(int index)
{
    this->command->setStep(this->comboBoxStep->itemData(index).toString());
}

void InspectorAutoWidget::speedChanged(int index)
{
    this->command->setSpeed(this->comboBoxSpeed->itemData(index).toString());
}

void InspectorAutoWidget::transitionChanged(int index)
{
    this->command->setTransition(this->comboBoxTransition->itemData(index).toString());
}