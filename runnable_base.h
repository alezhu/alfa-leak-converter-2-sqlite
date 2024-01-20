#include "context.h"
#include "loger.h"
#include "state.h"

#include <QRunnable>

#pragma once

class runnable_base:    public QRunnable
{
public:
    runnable_base(state_t * state, context *ctx);
    void run() override;
protected:
    state_t* m_state;
    context *m_ctx;
    virtual void _step() = 0;
    loger &m_log;
};
