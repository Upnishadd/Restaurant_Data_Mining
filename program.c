/* Restaurant recommender:
 *
 * Skeleton code written by Jianzhong Qi, April 2022
 * Edited by: Upnishadd Ksharma 1263652
 *
 */

#include <stdio.h>
#include "listops.c"
#include <string.h>
#include <stdlib.h>

#define CUSTOMER_ID_LEN 6            // Length of a customer id
#define CUSINE_LEN 20                // Length of a Cusine Name
#define RESTAURANT_NAME_LEN 100      // Max Number of Restaurants
/* stage heading */
#define HEADING "==========================Stage %d==========================\n"


/* typedefs */

// Struct for Restaurant Info
typedef struct {
    int id;
    double x;
    double y;
    int p_head;
    char cusine[CUSINE_LEN + 1];
    char name[RESTAURANT_NAME_LEN + 1];
} rest_t;

/****************************************************************/
/* function prototypes */

void print_stage_header(int stage);

// Stage Functions

int stage_one(rest_t restaurants[]);
list_t stage_two(int count, rest_t restaurants[]);
list_t stage_three(list_t c_list, rest_t restaurants[], int count);
list_t stage_four(list_t c_list, int count);

// Helper Functions

int update(struct node *head, data_t curr, int rest_index);
void printArray(struct node *head, int count);
int is_close(rest_t restaurants[], int i, int j);
void match_rest(rest_t restaurants[], int count, struct node *curr);
int sim_score(int arr1[], int arr2[], int count);
void sim_update(int curr[], int best[], int sec_best[],int count);
/****************************************************************/
// algorithms are awesome
/* main program controls all the action */

int
main(int argc, char *argv[]) {
    rest_t restaurants[RESTAURANT_NAME_LEN + 1];
    list_t c_list;
    
    // Stage 1 - Read Restaurant Records
    int count = stage_one(restaurants);
    
    // Remove Seperator Line
    scanf("%*s");
    
    // Stage 2 - Read Dining Transactions
    c_list = stage_two(count, restaurants);
    
    // Stage 3 - Recommend Based on Restaurant Similarity
    c_list = stage_three(c_list, restaurants, count);
    
    // Stage 4 - Recommend Based on Customer Similarity
    c_list = stage_four(c_list, count);
    
    // Free Malloc'ed Memory
    free_list(&c_list);
    return 0;
}

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
    printf(HEADING, stage_num);
}




                            /* STAGE FUNCTIONS */

int stage_one(rest_t restaurants[]) {
    print_stage_header(1);
    // Lowest price per head, its index, and running count of no. restaurants
    int low_p_head = 0; int low_index = 0; int count = 0;
    
    // Scan in all necessary data about the restaurants
    for (int i = 0;(scanf("%d", &restaurants[count].id) != 0) && i < 100;i++){
        scanf("%lf %lf", &restaurants[count].x, &restaurants[count].y);
        scanf("%d", &restaurants[count].p_head);
        scanf("%s", restaurants[count].cusine);
        scanf("%s", restaurants[count].name);
        
        // Intitalise the lowest val to start with the first restaurant's price
        if (low_p_head == 0) {
            low_p_head = restaurants[0].p_head;
        }
        /* 
        Iterate thru all restaurants, storing the lowest price and the 
        respective restaurants index in the array of restaurants
        */
        if (restaurants[count].p_head < low_p_head){
            low_p_head = restaurants[count].p_head; 
            low_index = count;  
        } count++;
    }
    printf("Number of restaurants: %d\n", count);
    printf("Restaurant with the smallest average price: %s\n\n", 
            restaurants[low_index].name);
    return count;
}


list_t 
stage_two(int count, rest_t restaurants[]) { 
    print_stage_header(2);
    
    // Create an empty list to which the Customer Nodes will be added
    list_t c_list = *make_empty_list();
    char c_id[CUSTOMER_ID_LEN+1]; int rest_id; int rest_index;
    
    // Scan in the customer ID and the Restaurant ID until no more
    while (scanf("%*s %s %d %*d", c_id, &rest_id) == 2){
        data_t curr;
        // Find the index for the restaurant in the Array of Restaurants
        for(int i = 0; i < count; i++){
            if(restaurants[i].id == rest_id) {
                rest_index = i;
                i = count;
            }
        }
        
        // Store the Customer Id in a Temporary Struct
        strcpy(curr.cust_id, c_id);
        
        // Create a "0" Array
        for(int i = 0; i < count; i++) {
            curr.visits[i] = 0;
        }
        
        /* 
        If the Customer isn't already in the list, "initalise" with a "0" array
            and update for the current restaurant.
        */
        if (update(c_list.head, curr, rest_index) == 0){
            c_list = *insert_at_foot(&c_list, curr);
            update(c_list.head, curr, rest_index);
        }
    }
    printArray(c_list.head, count);
    printf("\n");
    return c_list;
}


list_t 
stage_three(list_t c_list, rest_t restaurants[], int count) {
    print_stage_header(3);
    struct node *curr = c_list.head;
    
    // Iterate thru linked list
    while(curr != NULL){
        
        // Updates array based on most similar restaurants
        match_rest(restaurants, count, curr);
        
        curr = curr->next;
    }
    
    printArray(c_list.head, count);
    printf("\n");
    return c_list;
}


list_t stage_four(list_t c_list, int count){
    print_stage_header(4);
    struct node *curr = c_list.head;
    node_t empty;
    
    // Create a "0" Node
    for(int i = 0; i < count; i++) {empty.data.visits[i] = 0;}
    
    // Iterate thru all elements of linked list
    while (curr != NULL){
        
        struct node *temp = c_list.head;
        int match_1 = 0; struct node *best = NULL;
        int match_2 = 0; struct node *sec_best = NULL;
        
        // Iterate thru all elements of linked list (to compare against)
        while (temp != NULL){
            
            // Don't compare a customer with themselves
            if (strcmp(curr->data.cust_id, temp->data.cust_id)!=0) { 
                int score;
                score = sim_score(curr->data.visits, temp->data.visits, count);
                
                // Check if best match
                if(score > match_1) {
                    // Move old best down to second
                    match_2 = match_1; sec_best = best;
                    // Replace new best
                    best = temp; match_1 = score;
                }
                // Otherwise check if second best match
                else if (score > match_2) {
                    sec_best = temp; 
                    match_2 = score;
                    }
            }
            temp = temp -> next;
        }
        
        // If there is not enough matches, point to a the "0" node
        if (best == NULL) {best = &empty;}  
        if (sec_best == NULL) {sec_best = &empty;} 
        sim_update(curr->data.visits, best->data.visits, 
                    sec_best->data.visits, count);
        
        curr = curr -> next;
    }
    printArray(c_list.head, count);
    return c_list;
}

/*
                             TIME COMPLEXITY;
    The time complexity for stage 4 was found to be O(n^3) as n -> ∞.
    
    # As R,C,K -> ∞, they are equal to n where n -> ∞.

    The stage can be seperated into 3 distinct portions:
        Creating empty array - O(R) ~ O(n)
        Match similar customers and update array - O(C*[C*(R+K)]+R) ~ O(n^3)
        Printing array - O(R*C) ~ O(n^2)
    
    Matching Similar Customers and updating array can be seperated into 4 
    distinct portions;
        Iterating thru all Customers - O(C)
            -> Finding Similarity Score - O(R)
            -> K-Matches being taken - O(K)
        Updating Current Customer - O(R)
        => Time Complexity for this section would be O(C*[C*(R+K)]+R)
        => O(C*[C*(R+K)]+R) = O(C^2(R+K)+ R) ~ O(n^3)
    
    => Overall Time complexity for the Algorithm would be O(n^3)
   

*/


                           /* HELPER FUNCTIONS */


// Update the visiting array, by adding one to the restauarant at its index
/* 
    This Updating algorithm was modified from the search algorithm found at
        https://www.geeksforgeeks.org/search-an-element-in-a-linked-list
        -iterative-and-recursive/
*/

int update(struct node *head, data_t curr, int rest_index) {
    
    if (head == NULL)
        return 0;
        
    if (strcmp(head->data.cust_id, curr.cust_id) == 0){
        head->data.visits[rest_index]++;
        return 1;
    }
    
    return update(head->next, curr, rest_index);
}


// Iterate thru the linked list, print the cust_id and then it array
void printArray(struct node *head, int count) {
    struct node *temp = head;
    while(temp != NULL) {
        printf("%s:", temp->data.cust_id);
        for(int i = 0; i < count; i++) {
            // For Special cases, print respective Char, otherwise print number
            if(temp->data.visits[i] == -1) {
                printf("  -");
            }
            else if(temp->data.visits[i] == -2) {
                printf("  +");
            }
            else if(temp->data.visits[i] == -3) {
                printf("  *");
            }    
            else {printf("  %d", temp->data.visits[i]);}
            }
        printf("\n");
        temp = temp->next;
    }
}


// Determine if two restaurants are within 30 units of eachother

int is_close(rest_t restaurants[], int i, int j) {
    double x_1 = restaurants[i].x;
    double x_2 = restaurants[j].x;
    double y_1 = restaurants[i].y;
    double y_2 = restaurants[j].y;
    double dist_squared = (x_1-x_2)*(x_1-x_2)+(y_1-y_2)*(y_1-y_2);
    if(dist_squared > 900){
        return 0;
    }
    return 1;
}

// Updates array based on most similar restaurants
void match_rest(rest_t restaurants[], int count, struct node *curr) {
    // i is the index for the current restaurant
    for(int i = 0; i < count; i++){
            
        // skip unless restaurant has been visited
        if((curr->data.visits[i]) > 0){
                
            // iterate thru all univisited restaurants
            for(int j = 0; j < count; j++){
                if((curr->data.visits[j]) <= 0){
                    
                    // Match based on:    
                        
                    // Distance
                    if(is_close(restaurants, i, j) == 1){
                        curr->data.visits[j] = -1;
                    }
                    // Cusine
                    else if(strcmp(restaurants[i].cusine, 
                                   restaurants[j].cusine) == 0){
                        curr->data.visits[j] = -1;
                    }
                    // Price Per Head
                    else if(abs(restaurants[i].p_head - 
                                restaurants[j].p_head) <= 20) {
                        curr->data.visits[j] = -1;          
                    }
                }
            }
        }
    }
}

// Find the similarity score of two Arrays

int sim_score(int arr1[], int arr2[], int count){
    int score = 0;
    for(int i = 0; i < count; i++) {
        if (arr1[i] > 0 && arr2[i] > 0){
            score = score + (arr1[i] * arr2[i]);
        }
    }
    return score;
}


// Update the array based on the similarity found

void sim_update(int curr[], int best[], int sec_best[],int count){
    for(int i = 0; i < count; i++){
        if(curr[i] <= 0) {
            if (best[i] > 0) {
            curr[i]--;
            }
            if (sec_best[i] > 0) {
            curr[i]--;
            }
        }
    }
}