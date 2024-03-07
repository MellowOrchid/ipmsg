#ifndef CFG_INIT_H
#define CFG_INIT_H

class cfg
{
private:
    static void create_cfg();
    static void read_cfg();

public:
    cfg();
    ~cfg();
    static void init();
};

#endif
