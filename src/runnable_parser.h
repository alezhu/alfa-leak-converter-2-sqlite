#pragma once

#include "context.h"
#include "csv_parser.h"
#include "runnable_base.h"

class runnable_parser : public runnable_base
{
public:
    runnable_parser(state_t* state, context* ctx, csv_parser* parser,bool skipHeader,
                    std::function<void(quint64)> fnSetPos);
protected:
    void _step() override;
private:
    csv_parser* m_parser;
    bool m_end = true;
    bool m_header = true;
#ifdef CHECK_DOUBLES
    void _clear_old();
#endif
    std::function<void(quint64)> m_fnSetPos;
};
