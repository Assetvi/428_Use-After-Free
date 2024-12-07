#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Example 1: Basic Use-After-Free
void basic_use_after_free() {
    struct example {
        int data;
    };
    struct example* obj = malloc(sizeof(struct example));
    obj->data = 42;
    free(obj);
    printf("Accessing freed memory (Basic UAF): %d\n", obj->data); // UAF
}

// Example 2: Double-Free
void double_free_example() {
    int* ptr = malloc(sizeof(int));
    free(ptr);
    free(ptr); // Double-free
    printf("Double-free executed.\n");
}

// Example 3: Indirect Use-After-Free
void indirect_use_after_free() {
    int* ptr = malloc(sizeof(int));
    int* alias = ptr; // Alias pointer
    free(ptr);
    printf("Accessing freed memory through alias (Indirect UAF): %d\n", *alias);
}

// Example 4: Use-After-Reallocation
void use_after_reallocation() {
    int* ptr = malloc(sizeof(int));
    *ptr = 42;
    free(ptr);
    ptr = malloc(sizeof(int)); // Reallocation
    printf("Accessing reallocated memory: %d\n", *ptr);
}

// Example 5: Use-After-Scope
void use_after_scope() {
    struct example {
        int data;
    };
    struct example* obj;
    {
        struct example temp;
        temp.data = 42;
        obj = &temp; // Pointer to stack memory
    }
    printf("Accessing out-of-scope memory: %d\n", obj->data); // UAF
}

// Example 6: Use-After-Free in Linked List
void linked_list_uaf() {
    struct node {
        int data;
        struct node* next;
    };
    struct node* head = malloc(sizeof(struct node));
    head->next = malloc(sizeof(struct node));
    free(head->next);
    printf("Accessing freed node in linked list: %d\n", head->next->data); // UAF
}

// Example 7: Multi-Threaded UAF Example
int* shared_ptr; // Shared pointer across threads

void* thread1_func(void* arg) {
    free(shared_ptr); // Thread 1 frees the shared pointer
    return NULL;
}

void* thread2_func(void* arg) {
    printf("Accessing freed memory in thread 2: %d\n", *shared_ptr); // Thread 2 uses the freed pointer
    return NULL;
}

void multithreaded_uaf_simulation() {
    shared_ptr = malloc(sizeof(int)); // Allocate shared memory
    *shared_ptr = 42;

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

// Example 8: Mach Port UAF Example
struct mach_port {
    int ref_count;
    void (*handle_message)(); // Simulated message handler
};

void fake_service() {
    printf("Fake service executed via UAF!\n");
}

void mach_port_uaf_example() {
    // Step 1: Allocate and initialize a simulated Mach port
    struct mach_port* port = malloc(sizeof(struct mach_port));
    if (!port) {
        printf("Memory allocation failed.\n");
        return;
    }
    port->ref_count = 1;  // Initial reference count
    port->handle_message = NULL;

    // Step 2: Simulate leaking references
    printf("Leaking references...\n");
    for (int i = 0; i < 10; i++) {
        port->ref_count++; // Increment reference count
    }

    // Step 3: Decrement references to trigger UAF
    printf("Decrementing references to 0...\n");
    while (port->ref_count > 0) {
        port->ref_count--; // Decrement reference count
    }

    // Step 4: Simulate reallocating the freed memory
    struct mach_port* hijacked_port = malloc(sizeof(struct mach_port));
    if (!hijacked_port) {
        printf("Memory reallocation failed.\n");
        free(port);
        return;
    }
    hijacked_port->handle_message = fake_service; // Attacker-controlled function pointer

    // Step 5: Call the hijacked function pointer
    if (hijacked_port->handle_message) {
        hijacked_port->handle_message(); // Exploitation point
    }

    // Free memory
    free(port);
    free(hijacked_port);
}

// False Positive Example 1: Reuse of Memory
void reuse_memory_example() {
    int* ptr = malloc(sizeof(int));
    *ptr = 42;
    free(ptr);  // Memory is freed
    ptr = malloc(sizeof(int));  // Legitimately reallocated
    *ptr = 84;  // Valid usage
    printf("Reused memory safely: %d\n", *ptr);
    free(ptr);
}

// False Positive Example 2: Scoped Pointer Reassignment
void scoped_pointer_example() {
    int* ptr = NULL;
    {
        int local_data = 42;
        ptr = &local_data;  // Pointer to local scope
    }
    ptr = NULL;  // Nullifying prevents invalid access
    printf("Scoped pointer reassigned safely.\n");
}

// False Positive Example 3: Conditional Access After Free
void conditional_access_example() {
    int* ptr = malloc(sizeof(int));
    *ptr = 42;
    free(ptr);  // Memory is freed
    if (ptr != NULL) {  // This check avoids access
        ptr = NULL;
    }
    printf("Conditional access avoided UAF.\n");
}

// False Positive Example 4: Dangling Pointer Nullification
void dangling_pointer_nullified() {
    int* ptr = malloc(sizeof(int));
    *ptr = 42;
    free(ptr);  // Memory is freed
    ptr = NULL;  // Explicit nullification prevents UAF
    if (ptr) {
        printf("This line will never execute.\n");
    }
    else {
        printf("Dangling pointer safely nullified.\n");
    }
}

// Main function to run all examples
int main() {

    printf("Running Basic Use-After-Free Example:\n");
    basic_use_after_free();

    //printf("\nRunning Double-Free Example:\n");
    //double_free_example();

    printf("\nRunning Indirect Use-After-Free Example:\n");
    indirect_use_after_free();

    printf("\nRunning Use-After-Reallocation Example:\n");
    use_after_reallocation();

    printf("\nRunning Use-After-Scope Example:\n");
    use_after_scope();

    printf("\nRunning Linked List Use-After-Free Example:\n");
    linked_list_uaf();

    // Multi-threaded UAF Example
    printf("\nRunning Multi-Threaded Use-After-Free Example:\n");
    multithreaded_uaf_simulation();

    printf("\nRunning Mach Port UAF Example:\n");
    mach_port_uaf_example();

    // False Positive Tests
    printf("\nRunning Reuse Memory Example:\n");
    reuse_memory_example();

    printf("\nRunning Scoped Pointer Reassignment Example:\n");
    scoped_pointer_example();

    printf("\nRunning Conditional Access Example:\n");
    conditional_access_example();

    printf("\nRunning Dangling Pointer Nullification Example:\n");
    dangling_pointer_nullified();

    return 0;
}

