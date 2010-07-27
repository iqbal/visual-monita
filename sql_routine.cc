#include <stdio.h>
#include <string.h>
#pragma hdrstop

#define debug_query

#include "sql_routine.hh"

#define blob_kirim   256

#pragma package(smart_init)

unsigned int error_count = 0;
unsigned int query_count = 0;

MyQuery::~MyQuery()
{
    isConnected=false;
    Close();
}

void pesan_error(Glib::ustring  s)
{
    error_count++;
    printf("%s\n", s.c_str());
}


int MyQuery::Query(void)
{
    char ctemp[128];
    jum_row=0;
    try
    {
        nomer_error = mysql_query( &myDB, Text );
        
        if (!nomer_error)
        {
            myRES = mysql_store_result( &myDB);
            jum_row = (int) mysql_num_rows( myRES );
            jum_field = (int) mysql_num_fields( myRES );
            myFD = mysql_fetch_fields( myRES );
            
            //ambil row nya untuk yang pertama
            myROW = mysql_fetch_row( myRES );
            posisi_row = 0;
            query_count++;
            
            if(query_count>5)
            {
                printf("Query not clean (%d)\n",query_count);
            }
        }
        else
        {
            sprintf(ctemp, "Connection error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
            printf("Error saat query: %s\n",ctemp);
            return 2;
        }
        
    }
    catch(...)
    {
        printf("ERROR saat melakukan query");
        jum_row=0;
        return 1;
    }
    
    return 0;
}

void MyQuery::Buang(void)
{
    try
    {
        mysql_free_result( myRES );
        query_count--;
    }
    catch(...)
    {
        printf("error saat mengosongkan SQL cache\n");
    }
}

void MyQuery::Query_row(void)
{
    nomer_error = mysql_query( &myDB, Text );
    
    if (!nomer_error)
    {
        myRES = mysql_store_result( &myDB);
        jum_row = (int) mysql_num_rows( myRES );
        mysql_free_result( myRES );
    }
    else
    {
        printf("Last query ROW error !\n");
    }
    
}

int MyQuery::init(void)
{
    printf("\nInisialisasi MySQL...");
    int ret,res,res_ptr;
    unsigned int timeout = 7;
    
    isConnected=false;
    mysql_init(&myDB);
    
    printf("\n -Ganti setting timeout...");
    ret = mysql_options(&myDB, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout);	
    
    if(ret!=0)
        printf("Gagal\n\r");
    else
        printf("OK\n");
    
    if(mysql_real_connect( &myDB, server, user, psw, db, port_koneksi_SQL,
			   NULL, 0 ))
    {
      	printf("Connection success\n");
        isConnected=true;
      	return 1;
    } 
    else 
    {
        fprintf(stderr, "Connection failed\n");
        if (mysql_errno(&myDB)) 
        {
            fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
            sprintf(ctemp, "Connection error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
            return 0;
        }
    }
}

void MyQuery::Close(void)
{
    isConnected=false;
    mysql_close(&myDB);
}

Glib::ustring MyQuery::ambil_string(const char *field)
{
    int i;
    
    try
    {
        for (i=0; i<jum_field; i++)
        {
            if (strcmp(myFD[i].name, field)==0)
            {
                if ( (Glib::ustring) myROW[i] != "(null)")
                    return myROW[i];
                else
                    return "";
            }
        }
    }
    catch(...)
    {
        sprintf(ctemp, "Connection error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
        return "-1";
    }
}

void MyQuery::Next(void)
{
    myROW = mysql_fetch_row( myRES );
    posisi_row++;
    
#ifdef use_progress
    if (FormBarQuery != NULL)
    {
        FormBarQuery->Bar1->Position = posisi_row;
        if (posisi_row > (jum_row-1))
            FormBarQuery->Hide();
    }
#endif
    
#ifdef use_bar
    if (FormDatabase != NULL)
    {
        FormDatabase->bar1->Position = posisi_row;
    }
#endif
    
}

void MyQuery::Prev(void)
{
    myROW = mysql_fetch_row( myRES );
    posisi_row--;
}

void MyQuery::First(void)
{
    myROW = mysql_fetch_row( myRES );
    posisi_row=0;
}

void MyQuery::Last(void)
{
    myROW = mysql_fetch_row( myRES );
    posisi_row=jum_row-1;
}

int MyQuery::Insert(void)     //Jika error return 1, jika sukses return 0
{
    try
    {
        
        nomer_error = mysql_query( &myDB, Text );
        if ( nomer_error )
        {
            sprintf(ctemp, "Connection error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
            printf("ERROR:%s\n",ctemp);
            return 1;
        }
    }
    catch(...)
    {
        sprintf(ctemp, "Query error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
        printf("ERROR:%s\n",ctemp);
        return 1;
    }
    return 0;
}

void MyQuery::Replace(void)
{
    if ( mysql_query( &myDB, Text ))
    {
        printf("Error replace data\n");
    }
}

int MyQuery::Update(void)
{
    try
    {
        if ( mysql_query( &myDB, Text ))
        {
            sprintf(ctemp, "Query error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
            printf("ERROR:%s\n",ctemp);
            return -1;
        }
    }
    catch(...)
    {
       	printf("Error saat mengupdate data\n");
        return -2;
    }
    
    return 0;
}

bool MyQuery::Delete(void)
{

    try
    {
        if ( mysql_query( &myDB, Text ))
        {
            printf("error");
            return false;
        }
        else
            return true;
    }
    catch(...)
    {
        sprintf(ctemp, "Database error %d: %s\n", mysql_errno(&myDB),mysql_error(&myDB));
        printf("Error saat delete: %s\n",ctemp);
        return false;
    }
    
    return true;
}

void MyQuery::cek_field(void)
{
    int i;
    char out[128];
    
    for (i=0; i<jum_field; i++)
    {
        sprintf(out, "field = %s, L = %lu, type = %d, max = %lu", myFD[i].name,  myFD[i].length, myFD[i].type, myFD[i].max_length);
    }
}

bool MyQuery::cek_blob(char *field)
{
    int i;
    for (i=0; i<jum_field; i++)
    {
        if (strcmp(myFD[i].name, field)==0 && myFD[i].type == MYSQL_TYPE_BLOB)
        {
            if (myROW[i] != NULL) return true;
            else
                return false;
        }
    }
}

unsigned char * MyQuery::ambil_buf(char *field, int &jum)
{
    int i;
    int y;
    
    for (i=0; i<jum_field; i++)
    {
        if (strcmp(myFD[i].name, field)==0)
        {
            jum = myFD[i].max_length;
            return (unsigned char*) myROW[i];
        }
    }
    
    return 0;
}

unsigned long MyQuery::last_id(void)
{
    return mysql_insert_id(&myDB);
}

unsigned int MyQuery::Ping(void)
{
    return mysql_ping(&myDB);
}

