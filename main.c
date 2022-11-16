#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Stock {
    int day_index; 
    char name[100];
    float day_start_value;
    float day_end_value;
    int trading_volume;
} Stock;


void assign_stock_property(Stock* stock, int property_index, char* value) 
{
    switch (property_index)
    {
    case 0:
        stock->day_index = atoi(value);
        break;
    case 1:
        strcpy(stock->name, value);
        break;
    case 2:
        stock->day_start_value = atof(value);
        break;
    case 3: 
        stock->day_end_value = atof(value);
        break;
    case 4:
        stock->trading_volume = atoi(value);
    default:
        break;
    }
}

float calculate_stock_diff(Stock stock)
{
    return stock.day_end_value - stock.day_start_value;
}

void print_html(Stock* scanned_stocks, int line_count, Stock most_traded_stock)
{
    printf("<html>\n");
    printf("<body>\n");
    printf("<div>\n");

    if (most_traded_stock.trading_volume > -1) {
        printf("<h1>%s: highest volume</h1>\n", most_traded_stock.name);
        printf("<div>Day: %d</div>\n", most_traded_stock.day_index);
        printf("<div>Start price: %.2f</div>\n", most_traded_stock.day_start_value);
        printf("<div>End price: %.2f</div>\n", most_traded_stock.day_end_value);
        printf("<div>Volume: %d</div>\n", most_traded_stock.trading_volume);
    } else {
        printf("Ticker %s was not found\n", most_traded_stock.name);
    }

    printf("</div>\n");
    printf("<ul>\n");

    for (int i = line_count; i > 0; i--) {
        Stock stock = scanned_stocks[i - 1];

        bool is_tracked_stock = strcmp(stock.name, most_traded_stock.name) == 0; 

        printf(
            "<li>%sDay: %d, ticker: %s, start: %.2f, end: %.2f, diff: %.2f, volume: %d%s</li>\n",
            is_tracked_stock ? "<b>" : "",
            stock.day_index,
            stock.name,
            stock.day_start_value,
            stock.day_end_value,
            calculate_stock_diff(stock),
            stock.trading_volume,
            is_tracked_stock ? "</b>" : ""
        );
    }

    printf("</ul>\n");
    printf("</body>\n");
    printf("</html>\n");
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        printf("Wrong parameters");
        return 1;
    }

    char* stock_arg = argv[1]; 
    int line_count = atoi(argv[2]);

    char read_line[100];

    int line_index = 0;
    int word_index = 0;

    Stock most_traded_stock = {.day_index = -1, .day_start_value = -1, .day_end_value = -1, .trading_volume = -1};
    strcpy(most_traded_stock.name, stock_arg);

    Stock* scanned_stocks = malloc(line_count * sizeof(Stock));

    while (fgets(read_line, 100, stdin) != NULL && line_index < line_count) 
    {
        char* token = strtok(read_line, ",\n\0");

        Stock scanned_stock;

        do 
        {   
            assign_stock_property(&scanned_stock, word_index, token);
            word_index++;
        } while ((token = strtok(NULL, ",\0")) != NULL);

        scanned_stocks[line_index] = scanned_stock;

        if (strcmp(scanned_stock.name, stock_arg) == 0 && scanned_stock.trading_volume > most_traded_stock.trading_volume) {
            most_traded_stock = scanned_stock;
        }

        word_index = 0;
        line_index++;
    }

    print_html(scanned_stocks, line_count, most_traded_stock);

    free(scanned_stocks);
    scanned_stocks = NULL;

    return 0;
}