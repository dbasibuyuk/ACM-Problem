#include <stdio.h>
#include <stdlib.h>

struct Machines
{
    int day;
    int price;
    int resell;
    int profit;
    int g_until_end; /* finds the maximum profit of the machines until the last day */
};

struct testInfo
{
    int num_of_machine;
    int money;
    int num_of_days;
};

struct Company
{
    int own; /* if company has machine, it is 1, if company doesn't have, it is 0. */
    int today; /* it specify on which day we are */
    int budget;
    int profit_of_machine;/* if company has a machine, it shows the profit by day */
    int resell_of_machine;
};

void evaluate(struct Machines *machine, struct testInfo *test, int num_of_test);
void machine_sort(struct Machines *machine, int size, int last_day);
int machine_decider(struct Machines *machine, struct Company *company, struct testInfo *test, int num_of_test, int num_of_days);


int main()
{
    int num_of_machine;
    int num_of_test = 0;
    struct testInfo *test = malloc(1 * sizeof(struct testInfo));
    struct Machines *machine = malloc (1 * sizeof(struct Machines));
    
    
    FILE *fp = fopen("input.txt", "r");

    
    while(!feof(fp))
    {
        test = (struct testInfo *) realloc(test, (num_of_test + 1) * sizeof(struct testInfo));
        fscanf(fp, "%d %d %d", &test[num_of_test].num_of_machine, &test[num_of_test].money, &test[num_of_test].num_of_days);
        
        /* if it satisfies the final condition which is  0 0 0, program stops running. */
        if(test[num_of_test].num_of_machine == 0 && test[num_of_test].money == 0 && test[num_of_test].num_of_days == 0)
        {
            fseek(fp, 0, SEEK_END);
            getc(fp);
        }
        else
        {
            num_of_machine = 0;
            /* initializing machines for this case */
            machine = (struct Machines *) realloc(machine, test[num_of_test].num_of_machine * sizeof(struct Machines));
            for(int i = 0; i < test[num_of_test].num_of_machine; i++)
            {
                fscanf(fp, "%d %d %d %d", &machine[num_of_machine].day, &machine[num_of_machine].price, &machine[num_of_machine].resell, &machine[num_of_machine].profit);
                num_of_machine++;
            }
            evaluate(machine, test, num_of_test);
        }
        num_of_test++;
    }
    free(test);
    free(machine);
    return 0;
}



void evaluate(struct Machines *machine, struct testInfo *test, int num_of_test)
{
    int temp;
    struct Company *company = malloc(1 * sizeof(struct Company));
    company -> own = 0;
    company -> today = 1;
    company -> budget = test[num_of_test].money;
    if(test[num_of_test].num_of_machine > 1)
    {
        machine_sort(machine, test[num_of_test].num_of_machine, test[num_of_test].num_of_days);
        for(int i = 0; company -> today < test[num_of_test].num_of_days; i++)
        {
            machine_decider(machine, company, test, num_of_test, test[num_of_test].num_of_days);
        }
        printf("Case %d : %d\n", (num_of_test + 1), company -> budget);
    }
    else if(test[num_of_test].num_of_machine == 0)
    {
        printf("Case %d : %d\n", (num_of_test + 1), company -> budget);
    }
    else if(test[num_of_test].num_of_machine == 1)
    {
        machine[0].g_until_end = (test[num_of_test].num_of_days - machine[0].day) * machine[0].profit + machine[0].resell - machine[0].price;
        machine_decider(machine, company, test, num_of_test, test[num_of_test].num_of_days);
        printf("Case %d : %d\n", (num_of_test + 1), company -> budget);
    }
    
    free(company);
}

void machine_sort(struct Machines *machine, int size, int last_day)
{
    int temp_until_end, temp_day, temp_price, temp_resell, temp_profit, quicker_profit1, quicker_profit2;
    /* finds the maximum profit of the machines until the last day */
    for(int i = 0; i < size; i++)
    {
        machine[i].g_until_end = (last_day - machine[i].day) * machine[i].profit + machine[i].resell - machine[i].price;
    }
    for(int j = 0; j < size; j++)
    {
        for(int k = 0; k < size - 1; k++)
        {
            if(machine[k].g_until_end < machine[k + 1].g_until_end)
            {
                temp_until_end = machine[k].g_until_end;
                temp_day = machine[k].day;
                temp_price = machine[k].price;
                temp_resell = machine[k].resell;
                temp_profit = machine[k].profit;
                
                machine[k].g_until_end = machine[k + 1].g_until_end;
                machine[k].day = machine[k + 1].day;
                machine[k].price = machine[k + 1].price;
                machine[k].resell = machine[k + 1].resell;
                machine[k].profit = machine[k + 1].profit;
                
                machine[k + 1].g_until_end = temp_until_end;
                machine[k + 1].day = temp_day;
                machine[k + 1].price = temp_price;
                machine[k + 1].resell = temp_resell;
                machine[k + 1].profit = temp_profit;
            }
            /* if both machine can make the same profit in the end, then  we choose the one that has early release. */
            else if(machine[k].g_until_end == machine[k + 1].g_until_end)
            {
                if(machine[k].day > machine[k + 1].day)
                {
                    temp_until_end = machine[k].g_until_end;
                    temp_day = machine[k].day;
                    temp_price = machine[k].price;
                    temp_resell = machine[k].resell;
                    temp_profit = machine[k].profit;
                    
                    machine[k].g_until_end = machine[k + 1].g_until_end;
                    machine[k].day = machine[k + 1].day;
                    machine[k].price = machine[k + 1].price;
                    machine[k].resell = machine[k + 1].resell;
                    machine[k].profit = machine[k + 1].profit;
                    
                    machine[k + 1].g_until_end = temp_until_end;
                    machine[k + 1].day = temp_day;
                    machine[k + 1].price = temp_price;
                    machine[k + 1].resell = temp_resell;
                    machine[k + 1].profit = temp_profit;
                }
            }
        }
    }
}
/* return 0 means, company doesn't buy a machine. */
int machine_decider(struct Machines *machine, struct Company *company, struct testInfo *test, int num_of_test, int num_of_days)
{
    int flag;
    int temp_budget;
    int budget_w_buy; /* budget without buying */

    /* if company doesnt have machine */
    if(company -> own == 0)
    {
        budget_w_buy = company -> budget;
    }
    /* if company has a machine */
    else
    {
        budget_w_buy = company -> budget + company -> resell_of_machine + (test[num_of_test].num_of_days - company -> today) * company -> profit_of_machine;
    }
    
    for(int i = 0; i < test[num_of_test].num_of_machine; i++)
    {
        /* flag = 0 means, there is no need to buy a machine, flag = 1 means, company needs to buy this machine. */
        if(company -> own == 0)
        {
            if(budget_w_buy > machine[i].g_until_end)
            {
                flag = 0;
            }
            else
            {
                flag = 1;
            }
        }
        /* if company own a machine */
        else
        {
            if(company -> today == machine[i].day)
            {
                if(budget_w_buy > (company -> budget - company -> profit_of_machine + company -> resell_of_machine + machine[i].g_until_end))
                   {
                       flag = 0;
                   }
                else
                {
                    flag = 1;
                }
            }
            else if(company -> today < machine[i].day)
            {
                if(budget_w_buy > (company -> budget + (machine[i].day - company -> today - 1) * company -> profit_of_machine + company -> resell_of_machine + machine[i].g_until_end))
                {
                    flag = 0;
                }
                else
                {
                    flag = 1;
                }
            }
            else if(company -> today > machine[i].day)
            {
                flag = 0;
            }
        }
        
        
        /* if without buying a machine makes more profit than buying, company doesn't buy. */
        if(flag == 0)
        {
            if((i + 1) == test[num_of_test].num_of_machine)
            {
                company -> today = test[num_of_test].num_of_days;
                company -> budget = budget_w_buy;
            }
        }
        /* buying machine makes more profit than doing anything. */
        else
        {
            if(company -> own == 0)
            {
                if(company -> today <= machine[i].day)
                {
                    if(company -> budget >= machine[i].price)
                    {
                        company -> own = 1;
                        company -> budget = company  -> budget - machine[i].price + machine[i].profit;
                        company -> profit_of_machine = machine[i].profit;
                        company -> resell_of_machine = machine[i].resell;
                        company -> today = machine[i].day + 1;
                        return 1;
                    }
                }
            }
            else if(company -> own == 1)
            {
                if(company -> today == machine[i].day)
                {
                    temp_budget = company -> budget;
                    company -> budget = company -> budget + company -> resell_of_machine - company -> profit_of_machine - machine[i].price;
                    if(company -> budget >= 0)
                    {
                        company -> budget = company -> budget + machine[i].profit;
                        company -> today = machine[i].day + 1;
                        company -> profit_of_machine = machine[i].profit;
                        company -> resell_of_machine = machine[i].resell;
                        return 1;
                    }
                    else
                    {
                        company -> budget = temp_budget;
                    }
                }
                else if(company -> today < machine[i].day)
                {
                    temp_budget = company -> budget;
                    company -> budget = company -> budget + (machine[i].day - company -> today - 1) * company -> profit_of_machine + company -> resell_of_machine;
                    if(company -> budget >= machine[i].price)
                    {
                        company -> budget = company -> budget - machine[i].price + machine[i].profit;
                        company -> profit_of_machine = machine[i].profit;
                        company -> resell_of_machine = machine[i].resell;
                        company -> today = machine[i].day + 1;
                        return 1;
                    }
                    else
                    {
                        company -> budget = temp_budget;
                    }
                }
            }
        }
    }
    return 0;
}

