/****************************
Name: Jay J. Cruz
Date: 07/10/2020
Description: Payroll Program
****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "employee.h"
#define FLUSH while (getchar() != '\n')

int main() {
    int userChoice, fileChoice;
    int numEmployees = 0, employeeCount = 0;
    int hoursWorked;
    double hourlyRate;
    char repeatChoice, readFileChoice;
    EMPLOYEE** employee = NULL;
    
    do {
       displayTitle();
       displayPayrollMenu();
       switch (getUserPayrollChoice(userChoice)) {
          case 1:
            numEmployees = getNumEmployees(numEmployees);
            employee = calloc(numEmployees, sizeof(EMPLOYEE**));
            checkMemory(employee);
            addEmployees(employee, numEmployees);
            getEmployeeInfo(employee, numEmployees);
            fileChoice = getPayrollFileOption(fileChoice);
            makeOrAppendToFiles(fileChoice, employee, numEmployees);
            break;
         case 2:     
            // read from .txt file and display to screen
            printf("\nReading from text file:\n");
            readFromPayrollTxtFile();
            
            // read from .bin file if user chooses to
            if (askToViewBinaryFile(readFileChoice) == 'Y') {
                printf("\nReading from bin file:\n");
                readFromPayrollBinFile(numEmployees);
            }
            break;
         case 3:
            printf("\nExiting program...\n");
            exit(1);
         default:  
            printf("Invalid option entered...\n");
            break;
       }
       employeeCount++;
       
       if (employeeCount == numEmployees) {
          employeeCount = askToRepeatProgram(repeatChoice, employeeCount, numEmployees);
       }
    } while (employeeCount < numEmployees);

    freeMemory(employee, numEmployees);

  return 0;
}

// Function definitions

void displayTitle() {
   printf("\n******* PAYROLL PROGRAM *******\n");
} // end displayTitle

void displayPayrollMenu() {
  printf("\n1.Add a new pay record\n");
  printf("2.Display All payroll paycheck stubs to screen and text file\n");
  printf("3.Exit payroll program\n");
} // end displayPayRollMenu

EMPLOYEE* makeEmployee() {
    EMPLOYEE* employeeResult;

    employeeResult = malloc(sizeof(EMPLOYEE));

    return employeeResult;
} // end makeEmployee

int getUserPayrollChoice(int choice) {
   scanf("%i", &choice); FLUSH;
    
   return (int)choice;
} // end getUserPayrollChoice

int getPayrollFileOption(int choice) {
    printf("\nPlease select one of the below options:\n");
    printf("\n(1) Make/Overwrite employee pay record to files");
    printf("\n(2) Add employee pay record to existing to files\n");
    scanf("%i", &choice); FLUSH;

    return choice;
} // end getPayrollFileOption

void makeOrAppendToFiles(int choice, EMPLOYEE** employee, int numEmployees) {
    if (choice == 1) {
        // make employee pay record files
        addEmployeeToTxtFile(employee, numEmployees);
        addEmployeeToBinFile(employee, numEmployees);
        printf("\n\nPay records added:\n");
        displayEmployeePayRecords(employee, numEmployees);
    }
    else if (choice == 2) {
        // append employee pay records to exisiting files
        appendEmployeeToTxtFile(employee, numEmployees);
        appendEmployeeToBinFile(employee, numEmployees);
        printf("\n\nPay records added:\n");
        displayEmployeePayRecords(employee, numEmployees);
    }
    else {
        printf("Invalid option entered..\n");
        return;
    }
    return; 
} // end makeOrAppendToFiles

int askToRepeatProgram(char choice, int employeeCount, int numEmployees) {
    if (employeeCount == numEmployees) {
        if (getChoiceToRepeat(choice) == 'Y') {
            employeeCount = 0;
        }
        else {
            employeeCount = numEmployees;
        }
    }
    return employeeCount;
} // end askToRepeatProgram

char getChoiceToRepeat(char choice) {
   int validateInput = 0;
   while (validateInput == 0) {
      printf("\nDo you want to repeat the Payroll Program? Y/N\n");
      scanf("%c", &choice); FLUSH;
      choice = toupper(choice);
      validateInput = validateChoiceInput(&choice, validateInput);
   }
    return choice;
} // end getChoiceToRepeat

char askToViewBinaryFile(char choice) {
   int validateInput;

   validateInput = 0;
   while (validateInput == 0) {
      printf("\nWould you like to view employee .bin file? Y/N\n");
      scanf("%c", &choice); FLUSH;
      choice = toupper(choice);
      validateInput = validateChoiceInput(&choice, validateInput);
   }
   return choice;
} // end askToViewBinaryFile

int validateChoiceInput(char *choice, int validateInput) {

    if ( (*choice == 'Y') || (*choice == 'N') )
        validateInput = 1;
    else
        printf("Invalid choice please enter Y/N...\n");

    return validateInput;
}

void getEmployeeInfo(EMPLOYEE** employee, int numEmployees) {
    // get and store employee information
    for (int i = 0; i < numEmployees; i++) {
        printf("\nEnter employee's name: ");
        scanf("%[^\n]", employee[i]->name);
      
        printf("\nEnter employee's hourly rate: ");
        scanf("%lf", &employee[i]->hourlyRate); FLUSH;

        printf("\nEnter employee's hours worked: ");
        scanf("%i", &employee[i]->hoursWorked); FLUSH;   
    } 
} // end getEmployeeInfo

void addEmployees(EMPLOYEE** employee, int numEmployees) {
   for (int i = 0; i < numEmployees; i++) {
        employee[i] = makeEmployee();
    }
} // end addEmployes 

int getNumEmployees(int numEmployees) {
    printf("\nEnter number of employees: ");
    scanf("%i", &numEmployees); FLUSH;
    return numEmployees;
} // end getNumEmployees

void addPayRecord(EMPLOYEE* employee, double hourlyRate, int hoursWorked, int numEmployees) {
    double regularPay, overtimePay, grossPay, fedTaxDeduct, socialSecDeduct,
    netPay;

    // make all payroll calculations and deductions
    regularPay = calculateRegPay(employee->hourlyRate, employee->hoursWorked);
    overtimePay = calculateOTpay(employee->hourlyRate, employee->hoursWorked);
    grossPay = calculateGrossPay(regularPay, overtimePay);
    fedTaxDeduct = calculateFedTaxDeduct(grossPay);
    socialSecDeduct = calculateSocialSecDeduct(grossPay);
    netPay = calculateNetPay(grossPay, fedTaxDeduct, socialSecDeduct);

    // add payroll calculations and deductions to files
    addCalculationsToTxtFile(employee, grossPay, netPay, fedTaxDeduct, socialSecDeduct);
    addCalculationsToBinFile(employee, grossPay, netPay, fedTaxDeduct, socialSecDeduct);
    
    // output payroll calculations and deductions 
    displayPayCalculations(regularPay, overtimePay, grossPay, netPay);
    displayDeductions(fedTaxDeduct, socialSecDeduct);
} // end addPayRecord

void displayEmployee(EMPLOYEE* employee, int numEmployees) {
    printf("\n---------------------------------\n");
    printf("\nEmployee Name: %s", employee->name);
    printf("\nEmployee Hourly Rate: $%.2lf", employee->hourlyRate);
    printf("\nEmployee Hours Worked: %i Hrs", employee->hoursWorked);
    addPayRecord(employee, employee->hourlyRate, employee->hoursWorked, numEmployees);
    printf("\n----------------------------------\n");
} // end displayEmployee

void displayEmployeePayRecords(EMPLOYEE** employee, int numEmployees) {
    for (int i = 0; i < numEmployees; i++) {
        displayEmployee(employee[i], numEmployees);         
    }
} // end displayEmployeePayRecord

void displayPayCalculations(double regPay, double otPay, double grossPay, double netPay) {
    printf("\nRegular pay: $%.2lf", regPay);
    printf("\nOvertime pay: $%.2lf", otPay);
    printf("\nGross pay: $%.2lf", grossPay);
    printf("\nNet Pay: $%.2lf", netPay);
} // end displayPayCalculations

void displayDeductions(double fedTaxDeduct, double socialSecDeduct) {
    printf("\nFederal tax deduction: $%.2lf", fedTaxDeduct);
    printf("\nSocial security deduction: $%.2lf", socialSecDeduct);
} // end displayDeductions

void addEmployeeToTxtFile(EMPLOYEE** employees, int numEmployees) {
    FILE* employeeFilePtr;
    fpos_t filePosition;

    employeeFilePtr = fopen("employee_file.txt", "w");

    // get current position in stream 
    fgetpos(employeeFilePtr, &filePosition);
    checkIfFileIsNull(employeeFilePtr);

    // write employee's to .txt file
    for (int i = 0; i < numEmployees; i++) {
        fprintf(employeeFilePtr, "\nEmployee name: %s\n", employees[i]->name);
        fprintf(employeeFilePtr, "Employee hourly rate: $%.2lf\n", employees[i]->hourlyRate);
        fprintf(employeeFilePtr, "Employee hours worked: %i\n", employees[i]->hoursWorked);
        fprintf(employeeFilePtr, "Regular pay: $%.2lf\n", calculateRegPay(employees[i]->hourlyRate, employees[i]->hoursWorked));
        fprintf(employeeFilePtr, "Overtime pay: $%.2lf\n", calculateOTpay(employees[i]->hourlyRate, employees[i]->hoursWorked));
   }
   fsetpos(employeeFilePtr, &filePosition);
   fclose(employeeFilePtr);
} // end addToTextFile

void addCalculationsToTxtFile(EMPLOYEE* employee, double grossPay, double netPay, 
    double fedTaxDeduct, double socialSecDeduct) {
    FILE* calculationsFilePtr;
    fpos_t filePosition;
    int employeeIndex = 0;

    calculationsFilePtr = fopen("employee_file.txt", "a");
    fgetpos(calculationsFilePtr, &filePosition);
    checkIfFileIsNull(calculationsFilePtr);

    // append employee pay calculations to .txt file
    fprintf(calculationsFilePtr, "\n%s's Pay record calculations: ", employee[employeeIndex].name);
    fprintf(calculationsFilePtr, "\nGross pay: $%.2lf", grossPay);
    fprintf(calculationsFilePtr, "\nNet Pay: $%.2lf", netPay);
    fprintf(calculationsFilePtr, "\nFederal tax deduction: $%.2lf", fedTaxDeduct);
    fprintf(calculationsFilePtr, "\nSocial security deduction: $%.2lf\n", socialSecDeduct);
    employeeIndex++;

    fsetpos(calculationsFilePtr, &filePosition);
    fclose(calculationsFilePtr);
} // end addCalculationsToTxtFile

void appendEmployeeToTxtFile(EMPLOYEE** employees, int numEmployees) {
    FILE* employeeFilePtr;
    fpos_t filePosition;

    employeeFilePtr = fopen("employee_file.txt", "a");
    fgetpos(employeeFilePtr, &filePosition);
    checkIfFileIsNull(employeeFilePtr);

    for (int i = 0; i < numEmployees; i++) {
        fprintf(employeeFilePtr, "\nEmployee name: %s\n", employees[i]->name);
        fprintf(employeeFilePtr, "Employee hourly rate: $%.2lf\n", employees[i]->hourlyRate);
        fprintf(employeeFilePtr, "Employee hours worked: %i\n", employees[i]->hoursWorked);
        fprintf(employeeFilePtr, "Regular pay: $%.2lf\n", calculateRegPay(employees[i]->hourlyRate, employees[i]->hoursWorked));
        fprintf(employeeFilePtr, "Overtime pay: $%.2lf\n", calculateOTpay(employees[i]->hourlyRate, employees[i]->hoursWorked));
   }
   fsetpos(employeeFilePtr, &filePosition);
   fclose(employeeFilePtr);
} // end addToTextFile


void addEmployeeToBinFile(EMPLOYEE **employees, int numEmployees) {
   FILE* employeeBinFilePtr;
   fpos_t filePosition;

   employeeBinFilePtr = fopen("employee.bin", "wb");
   checkIfFileIsNull(employeeBinFilePtr);
   fgetpos(employeeBinFilePtr, &filePosition);

   // write to binary file 
   for (int i = 0; i < numEmployees; i++) {
      fwrite(&employees[i], sizeof(EMPLOYEE), numEmployees, employeeBinFilePtr);
   }
   fsetpos(employeeBinFilePtr, &filePosition);
   fclose(employeeBinFilePtr);
} // end addToBinFile

void addCalculationsToBinFile(EMPLOYEE* employee, double grossPay, double netPay, 
   double fedTaxDeduct, double socialSecDeduct) {
   FILE* calculationsBinFilePtr;
   fpos_t filePosition;
   int employeeIndex = 0;

   calculationsBinFilePtr = fopen("employee.bin", "ab");
   checkIfFileIsNull(calculationsBinFilePtr);
   fgetpos(calculationsBinFilePtr, &filePosition);

   // write to binary file 
   fwrite(&employee[employeeIndex], sizeof(EMPLOYEE), 1, calculationsBinFilePtr);
   fwrite(&grossPay, sizeof(double), 1, calculationsBinFilePtr);
   fwrite(&netPay, sizeof(double), 1, calculationsBinFilePtr);
   fwrite(&fedTaxDeduct, sizeof(double), 1, calculationsBinFilePtr);
   fwrite(&socialSecDeduct, sizeof(double), 1, calculationsBinFilePtr);
   employeeIndex++;
   
   fsetpos(calculationsBinFilePtr, &filePosition);
   fclose(calculationsBinFilePtr);
} // end addCalculationsToBinFile

void appendEmployeeToBinFile(EMPLOYEE **employees, int numEmployees) {
   FILE* employeeBinFilePtr;
   fpos_t filePosition;

   employeeBinFilePtr = fopen("employee.bin", "ab");
   checkIfFileIsNull(employeeBinFilePtr);
   fgetpos(employeeBinFilePtr, &filePosition);

   // write to binary file 
   for (int i = 0; i < numEmployees; i++) {
      fwrite(&employees[i], sizeof(EMPLOYEE), numEmployees, employeeBinFilePtr);
   }
   fsetpos(employeeBinFilePtr, &filePosition);
   fclose(employeeBinFilePtr);
} // end addToBinFile

void readFromPayrollTxtFile() {
    FILE* payrollFilePtr;
    char reader;

    payrollFilePtr = fopen("employee_file.txt", "r");
    checkIfFileIsNull(payrollFilePtr);

    // output text until end of file
    while ( (reader = getc(payrollFilePtr)) != EOF) {
        printf("%c", reader);
    }
    fclose(payrollFilePtr);
} // end readFromPayrollTxtFile

void readFromPayrollBinFile(int numEmployees) {
    FILE* payrollFilePtr;
    char* readResult;
    int fileCount;

    payrollFilePtr = fopen("employee.bin", "rb");
    checkIfFileIsNull(payrollFilePtr);

    // output bin file data 
    while (!feof(payrollFilePtr)) {
        fread(&readResult, sizeof(char), 1, payrollFilePtr);
        printf("%c", readResult[fileCount]);
        fileCount++;
    }
    fclose(payrollFilePtr);
} // end readFromPayrollBinFile

double calculateRegPay(double hourlyRate, int hoursWorked) {
    double regPayResult;
    if (hoursWorked <= 40) {
        regPayResult = (double)(hourlyRate * hoursWorked);
    }
    else {
        regPayResult = (double)((hourlyRate * hoursWorked) - (hoursWorked - 40));
    }
    return regPayResult;
} // end calculateRegPay

double calculateOTpay(double hourlyRate, int hoursWorked) {
    // calculate for all hours above 40 at time and a 1/2
    double oTpayResult;
    if (hoursWorked > 40) {
        // Hourly pay rate x 1.5 x overtime hours worked
        oTpayResult = (double)((hourlyRate * 1.5) * (hoursWorked - 40));
    }
    else {
        oTpayResult = 0.0;
        printf("\nNo overtime pay..");
    }
    return oTpayResult;
} // end calculateOTpay

double calculateGrossPay(double regularPay, double overtimePay) {
    double grossPayResult;

    //  Gross Pay (reg + ot)
    grossPayResult = (double)(regularPay + overtimePay);

    return grossPayResult;
} // end calcualteGrossPay

double calculateFedTaxDeduct(double grossPay) {
    double fedTaxDeductResult;

    // Fed Tax Deduction (28% of Gross Pay)
    fedTaxDeductResult = (28 * grossPay) / 100;

    return fedTaxDeductResult;
} // end calculateFedTaxDeduct

double calculateSocialSecDeduct(double grossPay) {
    double socialSecDeductResult;

    // Social Security Deduction (7.5% of Gross Pay)
    socialSecDeductResult = (7.5 * grossPay) / 100;

    return socialSecDeductResult;
} // end calculateSocialSecDeduct

double calculateNetPay(double grossPay, double fedTaxDeduct, double socialSecDeduct) {
    double netPayResult;

    // Net Pay (gross minus both deductions)
    netPayResult = grossPay - (fedTaxDeduct + socialSecDeduct);

    return netPayResult;
} // end calculateNetPay

void checkIfFileIsNull(FILE* file) {
    if (file == NULL) {
        printf("Error opening this file...Exiting\n");
        exit(-1); // Hard exit
   }
} // end checkIfFileIsNull

void checkMemory(EMPLOYEE** employee) {
    if (employee == NULL) {
        printf("Could not allocate enough memory..\n");
        printf("Exiting program...\n");
        exit(-1);
        return;
    }
} // end checkMemory

void freeMemory(EMPLOYEE** employee, int numEmployees) {
    for (int i = 0; i < numEmployees; i++) {
        free(employee[i]);
    }
    free(employee);
} // end freeMemory
