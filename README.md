# Restaurant_Data_Mining
The Victorian Dining and Entertainment Program offers 25% cash back on eligible dining and entertainment purchases across Victoria. Through this program, the Victorian government can harvest millions of dining and entertainment transactions, which are a gold mine of opportunities. The aim of this project was to identify Victorian's dining patterns so as to promote restaurants and help with the recovery of the local business. Here, we focus just on dining to simplify the assignment setting.

Data Format:

The input starts with a list of at least 1 and up to 99 restaurant records sorted by the restaurant IDs (e.g., ABNs) in ascending order. Each restaurant record takes one line with the following six fields:
- The restaurant ID, which is a unique 6-digit integer.
- The x-coordinate of the restaurant, which is a real number.
- The y-coordinate of the restaurant, which is a real number. For simplicity, all x- and y-coordinates have been normalised into the range of (0, 100).
- The average price per head, which is an integer.
- The cuisine type, which is a string of at least 1 and up to 20 lower-case letters.
- The restaurant name, which is a string of at least 1 and up to 100 (lower-case letters or `_') characters.

The list of restaurant records ends with a line of  ve `#'s which serves as a separator line. Then, the input continues with an unknown number (at least 1) of dining transactions sorted in ascending order of the (unique) transaction time. Each dining transaction takes one line with the following four fields:
- The transaction date and time, which uses the following format: Year:Month:Day:Hour:Minute:Second.
- The customer ID, which is a string of 6 lower-case letters (each unique ID represents a different customer).
- The restaurant ID, which is a 6-digit integer (and has appeared in the list of restaurants).
- The transaction amount, which is an integer.
