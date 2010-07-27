#ifndef SQL_ROUTINE_HH
#define SQL_ROUTINE_HH

#include <gtkmm.h>
#include <mysql.h>

#define port_koneksi_SQL   3306

class MyQuery
{
private:
    MYSQL		myDB ;
    MYSQL_RES	* myRES ;
    MYSQL_FIELD	* myFD ;
    MYSQL_ROW	myROW ;
    
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[2];
    
    int posisi_row;
    
public:
    ~MyQuery();
    
    char server[25];
    char user[25];
    char psw[25];
    char port[25];
    char db[25];
    
    char Text[2048];
    unsigned int  jum_row;
    unsigned int  jum_field;
    int init(void);
    char *buf_blob;
    unsigned long length1;
    int nomer_error;
    char pesan_error[128];
    char ctemp[256];
    
    //fungsi2
    void Close(void);
    void Open(void);
    int Query(void);
    void Query_row(void);
    void Buang(void);
    Glib::ustring ambil_string(const char *field);
    void Next(void);
    void Prev(void);
    void First(void);
    void Last(void);
    int Insert(void);
    void Replace(void);
    bool Delete(void);
    int Update(void);
    
    void cek_field(void);
    bool cek_blob(char *field);
    unsigned char * ambil_buf(char *field, int &jum);
    unsigned int CekUnclean (void);
    
    unsigned long last_id(void);
    unsigned int Ping(void);
    bool isConnected;
    
    unsigned int query_count;	
    
};

#endif //SQL_ROUTINE_HH

