#include<iostream>
#include<iomanip>
using namespace std;

//1990年1月1号是星期一
int nyear(int year)   //判断是闰年还是平年
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return 366;
    else
        return 365;
}

int nmonth(int y, int m)        //根据年份和月份判断是哪一个月的天数
{
    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
        return 31;
    else if (nyear(y) == 366 && m == 2)
        return 29;
    else if (nyear(y) == 365 && m == 2)
        return 28;
    else
        return 30;
}

int GetDays(int year, int month)  //输入年份和月份确定到1990.1.1总共多少天
{
    int i = 0;
    int sum = 0;
    if (year>1990)
    for (i = 1990; i < year; i++)
        sum += nyear(i);
    if (month>1)
    for (i = 1; i < month; i++)
    {
        sum += nmonth(year, i);
    }

    return sum;
}
void DisplayOnemonth()
{
    int i, j, year, month, day, sum, daycount, cases = 1;
    cout << "请输入-年-月：";
    cin >> year >> month;
    
        sum = GetDays(year, month);
        day = (sum + 1) % 7;  //得出一个月第一行前要空几格
        daycount = nmonth(year, month);   //算出这一个月的天数
        cout << "  " << year << "年" << month << "月" << endl;
        cout << "  日  一  二  三  四  五  六 " << endl;
        for (i = 0; i < day; i++)
            cout << setw(4) << " ";
        for (i = 1, j = day + i; i <= daycount; i++, j++)
        {
            cout << setw(4) << i;
            if (j % 7 == 0)
                cout << endl;
        }
        cout << endl;
    
}
void DisplayOneyear();

int main()
{
    DisplayOnemonth();  //调用函数
	DisplayOneyear();
    return 0;
}

void DisplayOneyear()
{
    int i = 0, year;
    int sum1, sum2, sum3;
    int day1, day2, day3;
    int daycount1, daycount2, daycount3;

    cout << "请输入年：";
    cin >> year;
    
        for (i = 1; i < 12; i += 3)
        {
            int j = 0, m = 1;   //控制打印日期的第一层
            int n = 0, q = 1;   //控制打印日期的第二层
            int k = 0, d = 1;   //控制打印日期的第三层
            sum1 = GetDays(year, i);          //到1990.1.1的总天数
            sum2 = GetDays(year, i + 1);
            sum3 = GetDays(year, i + 2);
            day1 = (sum1 + 1) % 7;            //得出一个月第一行前要空几格
            day2 = (sum2 + 1) % 7;
            day3 = (sum3 + 1) % 7;
            daycount1 = nmonth(year, i);       //一个月的总天数
            daycount2 = nmonth(year, i + 1);
            daycount3 = nmonth(year, i + 2);
            cout << setw(10) << " " << year << "年" << setw(2) << i << "月" << setw(21) << " " << "  " << year << "年" << setw(2) << i + 1 << "月" << setw(21) << " " << "  " << year << "年" << setw(2) << i + 2 << "月" << endl;
            cout << "  日  一  二  三  四  五  六 " << setw(4) << " " << "  日  一  二  三  四  五  六 " << setw(4) << " " << "  日  一  二  三  四  五  六 " << endl;
            for (j = 0; j < day1; j++)    //1.第一层循环，月份里第一行的空格数
                cout << setw(4) << " ";
            for (j = day1 + m; m <= daycount1+1; m++, j++)      //1.三层循环第一层
            {//1
                if (m>daycount1)       //多打印一行空格
                {
                    for (int a = 0; a < 7; a++)
                        cout << setw(4) << " ";
                    j = 7;
                }
                else
                    cout << setw(4) << m;

                if (m == daycount1)        //1.最后一行要的空格数
                {
                    for (int a = 0; a <((7 - (m + day1) % 7)%7); a++)
                        cout << setw(4) << " ";
                    j = 7;
                }
                if (j % 7 == 0)
                {
                    cout << setw(5) << " ";      //1.与下一个月相隔的距离
                    //#//--2
                    if (n == 0)     //2.当为月的第一行时
                    {
                        for (n = 0; n < day2; n++)       //2.第二层循环，月份里第一行的空格数     
                            cout << setw(4) << " ";
                    }
                    for (n = day2 + q; q <= daycount2+2; q++, n++)       //2.第二层循环
                    {//2
                        if (q>daycount2)      //多打印一行空格
                        {
                            for (int a = 0; a < 7;a++)
                                cout << setw(4) << " ";
                            n = 7;
                        }
                        else
                            cout << setw(4) << q;
                        if (q == daycount2)         //2.最后一行要的空格数
                        {
                            for (int a = 0; a <((7 - (q + day2) % 7)%7); a++)
                                cout << setw(4) << " ";
                            n = 7;               //2.使其可以进入下一个循环
                        }

                        if (n % 7 == 0)
                        {
                            q++;
                            n++;
                            cout << setw(4) << " "; //2.与下一个月相隔的距离
                            //#//--3
                            if (k == 0)           //3.月份的第一行
                            {
                                for (k = 0; k< day3; k++)      //3.月份的第一行要打印的空格数
                                    cout << setw(4) << " ";
                            }
                            for (k = day3 + d; d <= daycount3+1; d++, k++)   //3.打印月份的日期
                            {//3
                                if (d>daycount3)
                                {
                                    for (int a = 0; a < 7;a++)
                                        cout << setw(4) << " "; //多打印一行空格
                                    k = 7;
                                    m++;
                                    j++;
                                }
                                else
                                    cout << setw(4) << d;
                                if (d == daycount3)
                                {
                                    k++;
                                    d++;
                                    cout << endl;
                                    break;
                                }
                                if (k % 7 == 0)
                                {
                                    k++;
                                    d++;

                                    cout << endl;
                                    break;
                                }
                            }//3
                            break;
                        }
                    }//2
                }
            }//1
          
        }
    
}