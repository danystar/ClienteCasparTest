#include "RundownGroupWidget.h"

#include "Global.h"
#include "GpiManager.h"

#include "Events/RundownItemChangedEvent.h"

#include <QtGui/QApplication>
#include <QtCore/QObject>

RundownGroupWidget::RundownGroupWidget(const LibraryModel& model, QWidget* parent, bool active, bool compactView)
    : QWidget(parent),
      active(active), compactView(compactView), model(model)
{
    setupUi(this);

    setActive(active);
    setCompactView(compactView);

    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));

    this->labelLabel->setText(this->model.getLabel());

    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)),
                     this, SLOT(gpiDeviceConnected(bool, GpiDevice*)));

    checkGpiTriggerable();

    qApp->installEventFilter(this);
}

bool RundownGroupWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        RundownItemChangedEvent* rundownItemChangedEvent = dynamic_cast<RundownItemChangedEvent*>(event);
        this->model.setLabel(rundownItemChangedEvent->getLabel());
        this->model.setName(rundownItemChangedEvent->getName());

        this->labelLabel->setText(this->model.getLabel());
    }

    return QObject::eventFilter(target, event);
}

IRundownWidget* RundownGroupWidget::clone()
{
    RundownGroupWidget* widget = new RundownGroupWidget(this->model, this->parentWidget(), this->active, this->compactView);

    GroupCommand* command = dynamic_cast<GroupCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());

    return widget;
}

void RundownGroupWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelGroupColor->move(this->labelGroupColor->x(), Define::COMPACT_ITEM_HEIGHT - 2);
        this->labelThumbnail->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::COMPACT_VIEW_WIDTH, Define::COMPACT_VIEW_HEIGHT);
    }
    else
    {
        this->labelGroupColor->move(this->labelGroupColor->x(), Define::DEFAULT_ITEM_HEIGHT - 2);
        this->labelThumbnail->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::DEFAULT_VIEW_WIDTH, Define::DEFAULT_VIEW_HEIGHT);
    }

    this->compactView = compactView;
}

bool RundownGroupWidget::isGroup() const
{
    return true;
}

ICommand* RundownGroupWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownGroupWidget::getLibraryModel()
{
    return &this->model;
}

void RundownGroupWidget::setActive(bool active)
{
    this->active = active;

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownGroupWidget::setExpanded(bool expanded)
{
    //this->labelGroupColor->setVisible(expanded);
}

void RundownGroupWidget::channelChanged(int channel)
{

}

void RundownGroupWidget::videolayerChanged(int videolayer)
{

}

void RundownGroupWidget::delayChanged(int delay)
{

}

void RundownGroupWidget::checkGpiTriggerable()
{
    labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownGroupWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiTriggerable();
}

void RundownGroupWidget::gpiDeviceConnected(bool connected, GpiDevice* device)
{
    checkGpiTriggerable();
}