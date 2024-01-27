#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

int days_in_shamsi_month[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29, 29};

char *shamsiMonths[] = {
        " ",
         "               Farvardin              ",
         "              Ordibehesht             ",
         "                Khordad               ",
         "                  Tir                 ",
         "                 Mordad               ",
         "               Shahrivar              ",
         "                  Mehr                ",
         "                  Aban                ",
         "                  Azar                ",
         "                  Dey                 ",
         "                 Bahman               ",
         "                 Esfand               "
};

struct tm getCurrentDateAndTime(){
    time_t t = time(NULL);
    struct tm current_date = *localtime(&t);
    return current_date;
}

int inputCalendarYear(void)
{
    int year;
    int reTry;

    do
    {
        printf("\nPlease enter a year: ");
        scanf("%d", &year);

        if (year == 0)
        {
            return 0;
        }

        if (year < 1206 || year > 1498)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                system("cls");
            }
            if (reTry == 0)
            {
                return 0;
            }
        }
    } while (year < 1206 || year > 1498);

    return year;
}

int inputCalendarMonth(int year)
{
    int month;
    int reTry;

    do
    {
        printf("Please enter a month: ");
        scanf("%d", &month);
        if (month == 0)
        {
            return 0;
        }

        while (scanf("%d", &month)!=1 || month<1 || month>12)
        {
            printf("\n month: \n");
            scanf("%d", &month);
            
            
            if (reTry == 0)
            {
                return 0;
            }

            year = inputCalendarYear();

            if (year == 0)
            {
                return 0;
            }
        }
    } while (month < 1 || month > 12);

    return month;
}

int determineDaycode(int year, int month)
{
    int daycode = (year * 365 + (year / 4) + 1) % 7;

    if (year >= 1206 && year <= 1218)
    {
        daycode += 2;
    }

    if (year >= 1208 && year <= 1218 && year != 1211 && year != 1215)
    {
        daycode = (daycode - 1) % 7;
    }

    if (year == 1219)
    {
        daycode = (daycode + 2) % 7;
    }

    if (year >= 1220 && year < 1300)
    {
        daycode = (daycode + 1) % 7;
    }

    for (int i = 1; i < month; i++)
    {
        daycode = (daycode + days_in_shamsi_month[i]) % 7;
    }

    return daycode;
}

int determineLeapYear(int year)
{
    if ((year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0)))
        return 1;
    else
        return 0;
}

void calendar(int year, int month, int daycode)
{


    int day;
    int reTry;

    if (determineLeapYear(year) == 0)
    {
        days_in_shamsi_month[12] = 30;
    }
    else if (determineLeapYear(year) == 1)
    {
        days_in_shamsi_month[12] = 29;
    }



    printf("\n%s\n\nSH    YE    DO    SE    CH    PA    JO\n\n",
           shamsiMonths[month]
    );

    for (day = 1; day <= daycode; day++)
    {
        printf("      ");
    }

    for (day = 1; day <= days_in_shamsi_month[month]; day++)
    {
        printf("%-6d", day);

        if ((day + daycode) % 7 == 0 || day == days_in_shamsi_month[month])
            printf("\n");
    }

while(shamsiMonths[month]>12){
    printf("enter month again: ");
}

    printf("\n--------------------------------\n");
    printf("Enter 1 to try again: ");
    printf("enter month again: ");
    scanf("%d", &reTry);
    if (reTry == 1)
    {
        system("cls");
    }
}

void shamsiToGregorian(int y, int m, int d, int *gYear, int *gMonth, int *gDay)
{
    int sumShamsi[] = {31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336, 365};
    int gregorianDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int yy, mm, dayCount;
    dayCount = d;
    if (m > 1)
        dayCount = dayCount + sumShamsi[m - 2];

    yy = y + 621;
    dayCount = dayCount + 79;

    if (determineLeapYear(yy) == 1)
    {
        if (dayCount > 366)
        {
            dayCount = dayCount - 366;
            yy = yy + 1;
        }
    }
    else if (dayCount > 365)
    {
        dayCount = dayCount - 365;
        yy = yy + 1;
    }

    if (determineLeapYear(yy) == 1)
        gregorianDays[1] = 29;

    mm = 0;
    while (dayCount > gregorianDays[mm])
    {
        dayCount = dayCount - gregorianDays[mm];
        mm = mm + 1;
    }

    *gYear = yy;
    *gMonth = mm + 1;
    *gDay = dayCount;
}

void gregorianToShamsi(int year, int month, int day, int *sYear, int *sMonth, int *sDay)
{
    int countDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, dayYear;
    int newMonth, newYear, newDay;

    dayYear = 0;
    for (i = 1; i < month; i++)
    {
        dayYear = dayYear + countDays[i - 1];
    }
    dayYear = dayYear + day;

    if (determineLeapYear(year) && month > 2)
        dayYear = dayYear + 1;

    if (dayYear <= 79)
    {
        if ((year - 1) % 4 == 0)
        {
            dayYear = dayYear + 11;
        }
        else
        {
            dayYear = dayYear + 10;
        }

        newYear = year - 622;
        if (dayYear % 30 == 0)
        {
            newMonth = (dayYear / 30) + 9;
            newDay = 30;
        }
        else
        {
            newMonth = (dayYear / 30) + 10;
            newDay = dayYear % 30;
        }
    }
    else
    {
        newYear = year - 621;
        dayYear = dayYear - 79;

        if (dayYear <= 186)
        {
            if (dayYear % 31 == 0)
            {
                newMonth = dayYear / 31;
                newDay = 31;
            }
            else
            {
                newMonth = (dayYear / 31) + 1;
                newDay = dayYear % 31;
            }
        }
        else
        {
            dayYear = dayYear - 186;

            if (dayYear % 30 == 0)
            {
                newMonth = (dayYear / 30) + 7;
                newDay = 30;
            } else
            {
                newMonth = (dayYear / 30) + 8;
                newDay = dayYear % 30;
            }
        }
    }

    *sYear = newYear;
    *sMonth = newMonth;
    *sDay = newDay;
}

int intPart(double value)
{
    if (value < 0)
    {
        return (int)(value - 0.5);
    }
    else
    {
        return (int)(value + 0.5);
    }
}

void gregorianToLunar(int year, int month, int day, int *lYear, int *lMonth, int *lDay)
{
    int juliandate;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15))))
    {
        juliandate = intPart((1461 * (year + 4800 + intPart((month - 14) / 12))) / 4)
                     + intPart((367 * (month - 2 - 12 * intPart((month - 14) / 12))) / 12)
                     - intPart((3 * intPart((year + 4900 + intPart((month - 14) / 12)) / 100)) / 4)
                     + day - 32075;
    }
    else
    {
        juliandate = 367 * year
                     - intPart((7 * (year + 5001 + intPart((month - 9) / 7))) / 4)
                     + intPart((275 * month) / 9)
                     + day + 1729777;
    }

    int l = juliandate - 1948440 + 10632;
    int n = intPart((l - 1) / 10631);
    l = l - 10631 * n + 354;

    int j = (intPart((10985 - l) / 5316)) * (intPart((50 * l) / 17719))
            + (intPart(l / 5670)) * (intPart((43 * l) / 15238));

    l = l - (intPart((30 - j) / 15)) * (intPart((17719 * j) / 50))

        - (intPart(j / 16)) * (intPart((15238 * j) / 43)) + 29;

    int lunarM = intPart((24 * l) / 709);
    int lunarD = l - intPart((709 * lunarM) / 24);
    int lunarY = 30 * n + j - 30;

    *lYear = lunarY;
    *lMonth = lunarM;
    *lDay = lunarD;
}

void dateConversionMenu()
{
    printf("\n[0] Back to menu!\n\n[1] Shamsi to Gregorian/Lunar\n\n[2] Gregorian to Shamsi/Lunar");
    printf("\n\nselect option: ");
}

int dateConversion(void)
{
    int choice;
    int reTry;
    int sYear, sMonth, sDay;
    int gYear, gMonth, gDay;
    int lYear, lMonth, lDay;

    do
    {
        system("cls");

        dateConversionMenu();

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                continue;
            }
        }

        switch (choice)
        {
            case 1:
                system("cls");
                printf("\nYear: ");

                if (scanf("%d", &sYear) != 1 || sYear < 0)
                {
                    printf("\nInvalid input! Please enter a valid integer.\n");
                    printf("Press Enter 1 to try again: ");
                    scanf("%d", &reTry);
                    if (reTry == 1)
                    {
                        break;
                    }
                }

                if (sYear == 0)
                {
                    return -1;
                }

                printf("Month: ");

                if (scanf("%d", &sMonth) != 1 || sMonth < 1 || sMonth > 12)
                {
                    printf("\nInvalid input! Please enter a valid integer.\n");
                    printf("Press Enter 1 to try again: ");
                    scanf("%d", &reTry);
                    if (reTry == 1)
                    {
                        break;
                    }
                }

                printf("Day: ");

                if (scanf("%d", &sDay) != 1 || sDay < 1 || sDay > days_in_shamsi_month[sMonth])
                {
                    printf("\nInvalid input! Please enter a valid integer.\n");
                    printf("Press Enter 1 to try again: ");
                    scanf("%d", &reTry);
                    if (reTry == 1)
                    {
                        break;
                    }
                }

                shamsiToGregorian(sYear, sMonth, sDay, &gYear, &gMonth, &gDay);
                gregorianToLunar(gYear, gMonth, gDay, &lYear, &lMonth, &lDay);

                system("cls");
                printf("\nConverted Gregorian date: %d/%02d/%02d\n",
                gYear, gMonth, gDay);
                printf("\nConverted Lunar date: %d/%02d/%02d\n",
                lYear, lMonth, lDay);
                printf("Enter 1 to try again: ");
                scanf("%d", &reTry);
                if (reTry == 1)
                {
                    break;
                }

            case 2:
                system("cls");

                printf("\nYear: ");

                if (scanf("%d", &gYear) != 1 || gYear < 0)
                {
                    printf("\nInvalid input! Please enter a valid integer.\n");
                    printf("Press Enter 1 to try again: ");
                    scanf("%d", &reTry);
                    if (reTry == 1)
                    {
                        break;
                    }
                }

                if (gYear == 0)
                {
                    return -1;
                }

                printf("Month: ");

                if (scanf("%d", &gMonth) != 1 || gMonth < 1 || gMonth > 12)
                {
                    printf("\nInvalid input! Please enter a valid integer.\n");
                    printf("Press Enter 1 to try again: ");
                    scanf("%d", &reTry);
                    if (reTry == 1)
                    {
                        break;
                    }
                }

                printf("Day: ");

                if (scanf("%d", &gDay) != 1 || gDay < 1 || gDay > days_in_shamsi_month[gMonth])
                {
                    printf("\nInvalid input! Please enter a valid integer.\n");
                    printf("Press Enter 1 to try again: ");
                    scanf("%d", &reTry);
                    if (reTry == 1)
                    {
                        break;
                    }
                }

                gregorianToShamsi(gYear, gMonth, gDay, &sYear, &sMonth, &sDay);
                gregorianToLunar(gYear, gMonth, gDay, &lYear, &lMonth, &lDay);
                system("cls");
                printf("\nConverted Shamsi date: %d/%02d/%02d\n",
                sYear, sMonth, sDay);
                printf("\nConverted Lunar date: %d/%02d/%02d\n",
                lYear, lMonth, lDay);
                printf("Enter 1 to try again: ");
                scanf("%d", &reTry);
                if (reTry == 1)
                {
                    break;
                }

            case 0:
                return -1;

            default:
                printf("\nInvalid input! Please enter a valid integer.\n");
                printf("Press Enter 1 to try again: ");
                scanf("%d", &reTry);
                if (reTry == 1)
                {
                    break;
                }
        }

    } while (choice != 0);

    return 0;
}

void calculateAge(int birth_year, int birth_month, int birth_day)
{
    int reTry;
    system("cls");

    struct tm current_date = getCurrentDateAndTime();

    int current_gregorian_year = current_date.tm_year + 1900;
    int current_gregorian_month = current_date.tm_mon;
    int current_gregorian_day = current_date.tm_mday;

    int current_year, current_month, current_day;

    gregorianToShamsi(current_gregorian_year, current_gregorian_month, current_gregorian_day,
                      &current_year, &current_month, &current_day);

    if (birth_month < 1 || birth_month > 12)
    {
        printf("\nInvalid input! Please enter a valid integer.\n");
        printf("Press Enter 1 to try again: ");
        scanf("%d", &reTry);
        if (reTry == 1)
        {
            return;
        }
    }

    if (birth_day < 1 || birth_day > days_in_shamsi_month[birth_month])
    {
        printf("\nInvalid input! Please enter a valid integer.\n");
        printf("Press Enter 1 to try again: ");
        scanf("%d", &reTry);
        if (reTry == 1)
        {
            return;
        }
    }

    int age_years = current_year - birth_year;
    int age_months = current_month - birth_month;
    int age_days = current_day - birth_day;

    if (age_days < 0)
    {
        age_months--;
        age_days += days_in_shamsi_month[current_month];
    }

    if (age_months < 0)
    {
        age_years--;
        age_months += 12;
    }

    int birthdate_daycode = determineDaycode(birth_year, birth_month);
    int day_of_week_index = (birthdate_daycode + birth_day - 1) % 7;

    if (age_years < 0)
    {
        printf("\nInvalid input! Please enter a valid integer.\n");
        printf("Press Enter 1 to try again: ");
        scanf("%d", &reTry);
        if (reTry == 1)
        {
            return;
        }
    }

    char *days_of_week_shamsi[] = {"SHANBE", "YEKSHANBE", "DOSHANBE", "SESHANBE", "CHAHARSHANBE", "PANJESHANBE", "JOOMEH"};
    char *days_of_week_gregorian[] = {"SATURDAY", "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY"};

    int gYear, gMonth, gDay;
    shamsiToGregorian(birth_year, birth_month, birth_day, &gYear, &gMonth, &gDay);

    birth_year = gYear;
    birth_month = gMonth;
    birth_day = gDay;

    time_t birth_time = mktime(&(struct tm){0, 0, 0, birth_day, birth_month - 1, birth_year - 1900});
    time_t current_time = mktime(&current_date);

    int seconds_lived = difftime(current_time, birth_time);
    int days_lived = seconds_lived / (60 * 60 * 24);

    printf("\n Your Age: %d years, %d months, %d days", age_years, age_months, age_days);
    printf("\n You were born on %s\n", days_of_week_shamsi[day_of_week_index]);
    printf("\n Number of days passed since your birth: %d\n", days_lived);
    printf("\n Gregorian birth date: %d/%02d/%02d", gYear, gMonth, gDay);
    printf(" %s\n", days_of_week_gregorian[day_of_week_index]);
}

int age(void)
{
    int reTry;
    int birth_year, birth_month, birth_day;

    do
    {
        system("cls");

        printf("\nPlease enter the birth year: ");

        if (scanf("%d", &birth_year) != 1)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                system("cls");
                continue;
            };
        }

        if (birth_year == 0)
        {
            return 0;
        }

        if (birth_year < 1)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                system("cls");
                continue;
            };
        }

        printf("Please enter a month: ");

        if (scanf("%d", &birth_month) != 1)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                system("cls");
                continue;
            };
        }

        if (birth_month == 0)
        {
            return 0;
        }

        if (birth_month < 1 || birth_month > 12)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                system("cls");
                continue;
            };
        }

        printf("Please enter a day: ");

        if (scanf("%d", &birth_day) != 1)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                system("cls");
                continue;
            };
        }

        if (birth_day == 0)
        {
            return 0;
        }

        calculateAge(birth_year, birth_month, birth_day);

        printf("Enter 1 to try again: ");
        scanf("%d", &reTry);
        if (reTry == 1)
        {
            system("cls");
        }

    } while (1);
}

int main(void)
{
    int choice;
    int reTry;
    int shamsi_year, shamsi_month, shamsi_daycode;

    do {
        system("cls");

        printf("[0] Quit\n[1] Calendar\n[2] Age\n[3] Conversion\n");
        printf("-------------------------\n");
        printf("Select Option: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input! Please enter a valid integer.\n");
            printf("Press Enter 1 to try again: ");
            scanf("%d", &reTry);
            if (reTry == 1)
            {
                system("cls");
                continue;
            }
        }

        switch (choice)
        {
            case 0:
                system("cls");
                printf("Bye!\n");
                sleep(3);
                break;

            case 1:
                do {
                    system("cls");

                    shamsi_year = inputCalendarYear();

                    if (shamsi_year == 0)
                    {
                        break;
                    }

                    shamsi_month = inputCalendarMonth(shamsi_year);

                    if (shamsi_month == 0)
                    {
                        break;
                    }

                    shamsi_daycode = determineDaycode(shamsi_year, shamsi_month);
                    determineLeapYear(shamsi_year);

                    calendar(shamsi_year, shamsi_month, shamsi_daycode);
                }while(1);
                break;

            case 2:
                system("cls");
                age();
                break;

            case 3:
                system("cls");
                dateConversion();
                break;
        }
    }while(choice != 0);
}