#include <stdio.h>
#define MAXCHARS 50

typedef struct {
    char name[MAXCHARS];
    double hourlyRate;
    int hoursWorked;
} EMPLOYEE;

// Function Prototypes

void displayTitle();
void displayPayrollMenu();
int getUserPayrollChoice(int choice);
void getEmployeeInfo(EMPLOYEE**, int numEmployees);
EMPLOYEE* makeEmployee();
void addEmployees(EMPLOYEE**, int);
int getNumEmployees(int);
int getPayrollFileOption(int);
void displayEmployee(EMPLOYEE*, int);
void displayEmployeePayRecords(EMPLOYEE**, int);
void checkMemory(EMPLOYEE**);
void freeMemory(EMPLOYEE**, int);
void addPayRecord(EMPLOYEE*, double, int, int);
void makeOrAppendToFiles(int, EMPLOYEE**, int);
void addEmployeeToTxtFile(EMPLOYEE**, int);
void addCalculationsToTxtFile(EMPLOYEE*, double, double, double, double);
void addEmployeeToBinFile(EMPLOYEE**, int);
void addCalculationsToBinFile(EMPLOYEE*, double, double, double, double);
void appendEmployeeToTxtFile(EMPLOYEE**, int);
void appendEmployeeToBinFile(EMPLOYEE**, int);
void readFromPayrollTxtFile();
void readFromPayrollBinFile(int);
void checkIfFileIsNull(FILE*);
double calculateRegPay(double, int);
double calculateOTpay(double, int);
double calculateGrossPay(double, double);
double calculateFedTaxDeduct(double);
double calculateSocialSecDeduct(double);
double calculateNetPay(double, double, double);
void displayPayCalculations(double, double, double, double);
void displayDeductions(double, double);
int askToRepeatProgram(char, int, int);
char getChoiceToRepeat(char);
char askToViewBinaryFile(char);