#include "drawer.h"
#include <QGroupBox>
#include <QVBoxLayout>

Drawer::Drawer(QWidget *parent, Qt::WindowFlags f)
    :QToolBox(parent,f)
{
    setWindowTitle(tr("MyChat"));       //Set the title of main window

    toolBtn1 = new QToolButton;
    toolBtn1->setText(tr("用户1"));
    toolBtn1->setAutoRaise(true);
    toolBtn1->setToolButtonStyle(Qt::ToolButtonTextOnly);

    connect(toolBtn1,SIGNAL(clicked()),this,SLOT(showChatWidget1()));

    toolBtn2 = new QToolButton;
    toolBtn2->setText(tr("用户2"));
    toolBtn2->setAutoRaise(true);
    toolBtn2->setToolButtonStyle(Qt::ToolButtonTextOnly);

    connect(toolBtn2,SIGNAL(clicked()),this,SLOT(showChatWidget2()));

    toolBtn3 = new QToolButton;
    toolBtn3->setText(tr("用户3"));
    toolBtn3->setAutoRaise(true);
    toolBtn3->setToolButtonStyle(Qt::ToolButtonTextOnly);

    connect(toolBtn3,SIGNAL(clicked()),this,SLOT(showChatWidget3()));

    QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *layout = new QVBoxLayout(groupBox);

    layout->setMargin(20);      //Set the inter of the windows
    layout->setAlignment(Qt::AlignCenter);        //Align left
    layout->addWidget(toolBtn1);
    layout->addWidget(toolBtn2);
    layout->addWidget(toolBtn3);
    layout->addStretch();       //insert a tab
    this->addItem((QWidget*)groupBox,tr("选择账号进入："));
}

void Drawer::showChatWidget1()
{
    chatWidget1 = new Widget(0,toolBtn1->text());
    chatWidget1->setWindowTitle(toolBtn1->text());
    chatWidget1->show();
}

void Drawer::showChatWidget2()
{
    chatWidget2 = new Widget(0,toolBtn2->text());
    chatWidget2->setWindowTitle(toolBtn2->text());
    chatWidget2->show();
}

void Drawer::showChatWidget3()
{
    chatWidget3 = new Widget(0,toolBtn3->text());
    chatWidget3->setWindowTitle(toolBtn3->text());
    chatWidget3->show();
}
