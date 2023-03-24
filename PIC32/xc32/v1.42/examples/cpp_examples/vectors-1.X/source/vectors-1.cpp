/*-------------------------------------------------------------------------
 * This software is developed by Microchip Technology Inc. and its
 * subsidiaries ("Microchip").
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1.      Redistributions of source code must retain the above copyright
 *         notice, this list of conditions and the following disclaimer.
 * 2.      Redistributions in binary form must reproduce the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer in the documentation and/or other materials provided
 *         with the distribution.
 * 3.      Microchip's name may not be used to endorse or promote products
 *         derived from this software without specific prior written
 *         permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL MICROCHIP BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWSOEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *-------------------------------------------------------------------------*/

// A simple example of the vector container class
//    Example Expected output:
//    Begin test vectors-1.cpp
//    All numbers:
//    1 2 3 4
//    All numbers after push_back:
//    1 2 3 4 5 6 7 8
//    random_shuffle all numbers:
//    1 6 3 5 7 2 4 8
//    The largest number is 8
//    It is located at index 7
//    The number 5 is located at index 4
//    All numbers sorted:
//    1 2 3 4 5 6 7 8
//    Erasing all elements greater than 4...
//    Remaining numbers:
//    1 2 3 4
//    End vectors-1.cpp


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

// PIC32 devices have a software breakpoint instruction.
// Create a macro to use it easily with inline assembly.
#define PIC32_SOFTWARE_BREAK()  __asm__ volatile ("sdbbp 0")

#define SYS_FREQ 80000000ul

#include <xc.h>
#include <vector>
#include <algorithm> // sort, max_element, random_shuffle, remove_if, lower_bound
#include <functional> // greater, bind2nd
#include <cstdlib>
#include <iostream>

// used here for convenience, use judiciously in real programs.
using namespace std;

int main()
{
  int arr[4] = {1, 2, 3, 4};

  // Set stdout to go to UART 1 instead of UART 2
  // That way, we can see the output in the simulator's UART1 IO Window.
  __XC_UART=1;

  fprintf (stderr, "\nBegin test " __FILE__ "\n");

  // initialize a vector from an array
  vector<int> numbers(arr, arr+4);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Add code to configure cache, wait states, and peripheral bus clock
    // See http://www.microchip.com/mplabharmony

  cout << "All numbers:" << endl;

  // print all the numbers
  for(vector<int>::const_iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
      cout << *it << " ";
    }

  // insert more numbers into the vector
  numbers.push_back(5);
  numbers.push_back(6);
  numbers.push_back(7);
  numbers.push_back(8);
  // the vector currently holds {1, 2, 3, 4, 5, 6, 7, 8}

  cout << endl << "All numbers after push_back:" << endl;

  // print all the numbers
  for(vector<int>::const_iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
      cout << *it << " ";
    }

  // randomly shuffle the elements
  random_shuffle( numbers.begin(), numbers.end() );

  cout << endl << "random_shuffle all numbers:" << endl;

  // print all the numbers
  for(vector<int>::const_iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
      cout << *it << " ";
    }

  // locate the largest element, O(n)
  vector<int>::const_iterator largest =
    max_element( numbers.begin(), numbers.end() );

  cout << endl << "The largest number is " << *largest << endl;
  cout << "It is located at index " << largest - numbers.begin() << endl;

  // sort the elements
  sort( numbers.begin(), numbers.end() );

  // find the position of the number 5 in the vector
  vector<int>::const_iterator five =
    lower_bound( numbers.begin(), numbers.end(), 5 );

  cout << "The number 5 is located at index " << five - numbers.begin() << endl;

  cout << "All numbers sorted:" << endl;

  // print all the numbers
  for(vector<int>::const_iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
      cout << *it << " ";
    }

  cout << endl << "Erasing all elements greater than 4..." << endl;

  // erase all the elements greater than 4
  numbers.erase( remove_if(numbers.begin(), numbers.end(),
                           bind2nd(greater<int>(), 4) ), numbers.end() );

  cout << "Remaining numbers:" << endl;

  // print all the remaining numbers
  for(vector<int>::const_iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
      cout << *it << " ";
    }
  cout << endl; // Flush stdout with the newline

  cerr << "End " __FILE__ << endl << endl;

  PIC32_SOFTWARE_BREAK();
  while(1);
  return 0;
}
