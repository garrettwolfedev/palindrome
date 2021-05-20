/*
   Author: Garrett Wolfe
   Title: "Palindrome"
   Description: This program reads up to
    100 characters in the form of a c-string and
    determines whether the c-string is a palindrome.

 * Data Abstraction:

   The following variables are initialized:
    - 1 const int, MAX
    - 1 boolean, isPalindrome
    - 1 int, n
    - 1 char array, buffer

 * Input:

   The user is prompted to enter a 100 MAX character c-string
      into the prompt.

 * Process:

   The asm function, asmPalindrome, is called which parses through
      each character in the c-string by left and right indexed
      elements. Each char is converted to lowercase and determined
      if alphanumeric, if not, their address is moved to one that
      contains a valid char.

 * Output:

   The program will output a boolean indicating whether or not
      the input user string is a Palindrome.

 * Assumptions:

   - It is assumed the user wants to determine Palindrome.
   - It is assumed the user does not enter over 100 characters.
   - It is assumed the user will correctly input the
      data.
 */

#include <iostream>

using namespace std;

const int MAX = 100;

void __declspec(naked) asmPalindrome(const char[], int, bool&)
{
  __asm
  {
  START:
      push  ebp             //push ebp to stack
      mov   ebp, esp       //point esp to base pointer
      pushad              //push all registers to stack
      pushfd           //push EFLAGS to stack

      mov   ebx, [ebp + 8]    //point ebx to c-string
      mov   ecx, [ebp + 12]  //init ecx to c-string length
      mov   edi, [ebp + 8]  //est pointer to list (rightmost)
      mov   esi, [ebp + 16] //point esi to boolean ptr

      sub   eax, eax       //clear out eax for use
      sub   edx, edx      //clear out edx for use

      add   edi, ecx      //mov pointer to last element
      dec   edi          //est edi address to char prior to '\0'
      jecxz DONE        //if length is 0, we're DONE

  PARSE:
      cmp   ebx, edi      //compare left-most char addr to right-most char addr
      jg    DONE         //if if right-most has gone past left-most, we're DONE

  CHECKLEFT:
      mov   al, [ebx]   //move current left char into al
      or    al, 20h    //convert to lowercase
      cmp   al, 7Ah   //check char against z
      jae   NOTLEFT  //if above or equal, it's not char
      cmp   al, 61h //else, check against 'a'
      jae   CHECKRIGHT  //if not below a, continue to right

  NOTLEFT:
      inc   ebx    //move to next char in array
      jmp   PARSE //jump to top and continue parse

  CHECKRIGHT:
      mov   al, [edi]   //move current right char into al
      or    al, 20h    //convert to lowercase
      cmp   al, 7Ah   //check char against z 
      jae   NOTRIGHT //if above or equal, it's not char
      cmp   al, 61h //compare char to a
      jae   POST   //if not below, continue

  NOTRIGHT:
      dec   edi  //else, dec position for right-most array elem
      jmp   PARSE  //jump back to parse

  POST:
      mov   al, [ebx]   //move current left char into al
      mov   dl, [edi]  //move current right char into dl
      or    al, 20h   //convert left to lowercase
      or    dl, 20h  //convert right to lowercase
      cmp   al, dl  //compare left to right char
      jne   DONE    //if not equal, we're DONE

  CONTINUE:
      mov   [esi], 1   //while equal, set bool to true
      inc   ebx     //move forward from left in array
      dec   edi    //move backward from right in array

      loop  PARSE //loop back to top while ecx > 0

  DONE:
      popfd                //pop status flags from stack
      popad             //restore registers
      pop   ebp         //pop ebp from stack
      ret            //return to calling code
  }
}

int main()
{
  char buffer[MAX];							// our prospective palindrome
  int length = 0;								// length of the buffer
  bool isPalindrome = 0;				// initialize to FALSE

  cout << "Enter a string of text (<=100 chars): \n";
  cin.get(buffer, 100);

  length = strlen(buffer);

  asmPalindrome(buffer, length, isPalindrome);

  if (isPalindrome) {
    cout << "\nYour string IS a palindrome!\n\n";
  } else {
    cout << "\nYour string IS NOT a palindrome!\n\n";
  }
  return 0;
}