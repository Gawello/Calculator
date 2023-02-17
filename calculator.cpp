#include "calculator.h"
#include "./ui_calculator.h"
#include <QRegularExpression>



// Current value of calcution
double calcVal = 0.0;
// Will define if this was the last math button clicked
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

// Constructor
Calculator::Calculator(QWidget *parent)
    // Call the QMainWidnow constuctor
    : QMainWindow(parent)
    // Create the UI class and assign it to the ui member
    , ui(new Ui::Calculator)
{
    // Setup the UI
    ui->setupUi(this);

    // Put 0.0 in display
    ui->Display->setText(QString::number(calcVal));
    // Will hold references to all the number buttons
    QPushButton *numButtons[10];

    // Loop through locating buttons
    for(int i = 0; i < 10; ++i){
        QString butName = "Button" + QString::number(i);
        // Get the buttons by name and add to array
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        // When the button is released call NumPressed()
        connect(numButtons[i], SIGNAL(relasesed()), this, SLOT(NumPressed()));
    }

    // Connect signals and slots fot math buttons
    connect(ui->Add, SIGNAL(relasesed()), this, SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(relasesed()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(relasesed()), this, SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(relasesed()), this, SLOT(MathButtonPressed()));

    // Connect equals button
    connect(ui->Equals, SIGNAL(relasesed()), this, SLOT(MathButtonPressed()));
    // Connect change sign
    connect(ui->ChangeSign, SIGNAL(relasesed()), this, SLOT(ChangeNumberSign()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){
    // Sneder returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();
    // Get number on button
    QString butVal = button->text();
    // Get the value in the display
    QString displayVal = ui->Display->text();

    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){
        // caclVal = butVal.toDouble();
        ui->Display->setText(butVal);
    } else {
        // Put the new number to the right of whats there
        QString newVal = displayVal + butVal;
        // Double version of number
        double dblNewVal = newVal.toDouble();

        // Set value in display and allow up to 16
        // digits before using exponents
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}


void Calculator::MathButtonPressed(){

    // Cancel out previous math button clicks
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    // Store current value in Display
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();

    // Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();

    // Get math symbol on the button
    QString butVal = button->text();

    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
    } else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    } else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    } else {
        subTrigger = true;
    }

    // Clear display
    ui->Display->setText("");
}

void Calculator::EqualButtonPressed(){

    // Hold new calculation
    double solution = 0.0;

    // Get value in display
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    // Make sure a math button was pressed
    if (addTrigger || subTrigger || multTrigger || divTrigger){
        if (addTrigger){
            solution = calcVal + dblDisplayVal;
        } else if (subTrigger){
            solution = calcVal - dblDisplayVal;
        } else if (multTrigger){
            solution = calcVal * dblDisplayVal;
        } else {
            solution = calcVal / dblDisplayVal;
        }
    }

    // Put solutuion in display
    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign() {

    // Get the value in the display
    QString displayVal = ui->Display->text();

    // Regular expression checks if it is a number
    QRegularExpression reg("[-]?[0-9.]*");
    QRegularExpressionMatch match = reg.match(displayVal);

    // If it is a number change the sign
    if(match.hasMatch()) {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;

        // Put solutuon in display
        ui->Display->setText(QString::number(dblDisplayValSign));
    }

 }



