#pragma once

#include "database.h"
#include "runnable_base.h"

class runnable_db : public runnable_base
{
public:
    runnable_db(state_t * state, context *ctx, database* db);


private:
    database* m_db;
   void _insertChunks(strings* strings) const;
protected:
    void _step() override;

};
