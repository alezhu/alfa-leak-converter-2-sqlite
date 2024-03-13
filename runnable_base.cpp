#include "runnable_base.h"
#include <QThread>

runnable_base::runnable_base(state_t *state, context *ctx)
    : QRunnable(), m_state{state}, m_ctx{ctx}, m_log{loger::get()} {}

void runnable_base::run() {
  while (!m_state->stop) {
    _step();
    while (m_state->pause && !m_state->stop) {
        _sleep();
    }
  }
}

void runnable_base::_sleep()
{
    QThread::usleep(1);
    QThread::yieldCurrentThread();
}
