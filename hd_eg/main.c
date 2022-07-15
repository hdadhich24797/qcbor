#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

#include "address2.pb-c.h"

int main()
{
    Tutorial__Person p = TUTORIAL__PERSON__INIT;
    uint8_t *out;

    /*  Serialize   */
    p.name = (char *) malloc(20);

    strcpy(p.name, "Hrishikesh\0");
    p.id = 24;

    int len = tutorial__person__get_packed_size(&p);
    printf("len is %d\r\n", len);

    out = (uint8_t *) malloc(len);

    int packed_len = tutorial__person__pack(&p, out);
    printf("packed len is %d\r\n", packed_len);

    printf("packed data is %s %d\r\n", p.name, p.id);

    /*  De serialize    */
    Tutorial__Person *de = NULL;

    de = tutorial__person__unpack(NULL, packed_len, out);
    if (de == NULL)
        printf("error in de-serializing\r\n");

    printf("unpacked data is %s %d\r\n", de->name, de->id);
    tutorial__person__free_unpacked(de, NULL);
    return 0;
}
