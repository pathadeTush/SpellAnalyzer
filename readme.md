### Steps to run:

#### 1. First build this project using following command

         Syntax: ``` make ```
                  OR
         Syntax: ``` make all ```

         It will create inetrmediate files and executable file named 'SpellAnalyzer'

#### 2. There are three function which you can perform in this project

         i. Check text file for misspelled words and correction
         Syntax: ``` ./SpellAnalyzer /path/to/text_file.txt ```

         ii. Check for a single word
         Syntax: ``` ./SpellAnalyzer -w WordToBeChecked ```

         iii. Search for word with given prefix
         Syntax: ``` ./SpellAnalyzer -p prefixWord ```

#### 3. To remove all intermediate files generated

   Syntax: ``` make clean ```
