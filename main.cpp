#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "user_interface_xlib.cpp"
#include "main.h"
#include <cstring>
#include <array> // C++11 array

unsigned int epoch = 0;

int main(int argc, char **argv)
{
    std::cout << "Extensible Cellular Automata Engine" << std::endl;
    
//     start_gui();
    
    Grid* grid = new Grid;
    size_t window = 25;
    grid->resize(window, std::vector<bool>(window));
    
//     reserve_space(&grid, window);
    
    std::cout << grid->capacity() << std::endl;
    
//      for (int i = 0; i < window; i++)
//          for (int j = 0; j < window; j++)
//              grid[i][j].operator=(false);

//     (manually) configure starting grid (for now)
//     (*grid)[window/2][window/2] = true;
//     (*grid)[window/2-1][window/2] = true;
//     (*grid)[window/2][window/2+1] = true;
//     (*grid)[window/2][window/2-1] = true;
//     (*grid)[window/2+1][window/2+1] = true;
    
    // glider
    (*grid)[window/2][window/2] = true;
    (*grid)[window/2][window/2-1] = true;
    (*grid)[window/2][window/2+1] = true;
    (*grid)[window/2-1][window/2+1] = true;
    (*grid)[window/2-2][window/2] = true;
    

    // process the cellular universe and print our results
    while (true){
        printGrid(grid, window); // print results from current (or starting) generation
        
        update(&grid, window); // calculate the state of the grid after another epoch
        
        sleep(1); // wait one second so humans can see the result of the display
    
    #ifdef _WIN32 // clear the screen
        system("cls"); // on Windows
    #else
        system("clear"); // on Linux/BSD
    #endif
    }
    
    return 0;
}

void printGrid(Grid* grid, unsigned int window){
    std::cout << "Epoch " + std::to_string(epoch) << std::endl;
    std::cout << "Grid size: " << grid->size() << '\n';
    
    // handle top row
    for (int i = 0; i < window * 2; i++) {
        std::cout << '_';
    }
    std::cout << '\n';
    
    for(int a = 0; a < window; a++){ // display rows
        for(int b = 0; b < window; b++){ // display columns

            // handle beginning of rows
            if (b == 0) {
                std::cout << "|";
            }
            // handle live condition
            if ((*grid)[a][b] == true){
                std::cout << " *";
            }
            // handle dead condition
            else {
                std::cout << "  ";
            }
            // handle end of line
            if (b == window - 1){
                std::cout << '|' << std::endl;
            }
        }
    }
//     handle bottom row
    for (int i = 0; i < window * 2; i++) {
        std::cout << '_';
    }
    std::cout << '\n';
}

// function to calculate the grid's next state
void update(Grid** current, size_t window){
    
    Grid* next = new Grid;
    next->resize(window, std::vector<bool>(window));


//     reserve_space(&next, current->size() + 2);
    
    // main update loop
    for(int a = 0; a < window; a++){
        for(int b = 0; b < window; b++){
                (*next)[a][b] = rule_gol((**current)[a][b], neighbors(*current, a, b, window));
//                 (*next)[a][b] = rule_recurse2((**current)[a][b], neighbors(*current, a, b, window), current, window);
        }
    }
    
    free(*current);

    *current = next; // swap pointers
    
    epoch++; // increment epoch counter
}

size_t neighbors(Grid* grid, unsigned int x, unsigned int y, size_t window)
{    
    unsigned int count = 0;
    
    signed int x2 = (signed int) x;
    signed int y2 = (signed int) y;
    
    for (int i = x2 - 1; i <= x2 + 1; i++) {
        for (int j = y2 - 1; j <= y2 + 1; j++) {
            if (i == x2 && j == y2) {
                continue;
            }
            
            count += (*grid)[(i + window) % window][(j + window) % window];
        }
    }
    return count;
    
}

// bool rule(unsigned int num_neighbors, bool current_state, unsigned int x, unsigned int y)
bool rule_gol(bool live, size_t num_neighbors) {
    if (live) {
        if (num_neighbors < 2)
            return false;
        else if (num_neighbors == 2 || num_neighbors == 3)
            return true;
        else if (num_neighbors > 3)
            return false;
    } else {
        if (num_neighbors == 3)
            return true;
        else return false;
    }
    
}

bool rule_recurse1(bool live, size_t num_neighbors, Grid** grid, size_t window) {
    unsigned int sum = 0;
    
    for (unsigned int i = 0; i < window; i++)
        for (unsigned int j = 0; j < window; j++)
            sum += (**grid)[i][j];
    
    if ((sum % 8) < num_neighbors)
        return true; 
    else if ((sum % 8) > num_neighbors)
        return false;
    else
        return live;
}

bool rule_recurse2(bool live, size_t num_neighbors, Grid** grid, size_t window) {
//     std::cout << __PRETTY_FUNCTION__ << '\n';
    
    std::vector<unsigned int> column_sum;
    std::vector<unsigned int> row_sum;
    
    column_sum.resize(window);
    row_sum.resize(window);
    
    for (unsigned int i = 0; i < window; i++) { // rows
        for (unsigned int j = 0; j < window; j++) { // columns
            column_sum[i] += (**grid)[i][j];
//              column_sum[i] = column_sum[i] % 2;
            row_sum[j] += (**grid)[i][j];
//             row_sum[j] = row_sum[j] % 2;
        }
    }
    
    unsigned int horizontal_sum = 0;
    
    for (auto element : row_sum) {
        horizontal_sum += element;
    }
    
     unsigned int vertical_sum = 0;
    
    for (auto element : column_sum) {
        vertical_sum += element;
    }

    int discriminator = horizontal_sum ^ vertical_sum;
     
    discriminator = discriminator % 8;
    
    if (discriminator < num_neighbors)
        return true;
    else if (discriminator > num_neighbors)
        return false;
    else
        return live;
}
