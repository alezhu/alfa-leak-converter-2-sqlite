#include "runnable_parser.h"
#include "common.h"
#include <QDateTime>

runnable_parser::runnable_parser(state_t *state, context *ctx,
                                 csv_parser *parser,
                                 std::function<void(quint64)> fnSetPos)
    : runnable_base{state, ctx}, m_parser{parser}, m_fnSetPos{fnSetPos} {
  m_log << "Start parsing file";
}

void runnable_parser::_step() {
  if (!m_parser->next()) {
    m_state->stop = true;
    m_state->done = true;
    return;
  }

  if (m_header) {
    m_header = !m_header;
    return;
  }

  _clear_old();

  auto &loaded = m_ctx->loaded();
  auto user_id = m_parser->value(csv_parser::fields::user_id).toULongLong();
  auto user_exists = loaded.find(user_id);
  if (user_exists == loaded.end()) {
    auto name = m_parser->value(csv_parser::fields::name);
    auto birthDate = toDate(m_parser->value(csv_parser::fields::birth_date));

#ifdef CHECK_DOUBLES
    loaded.insert(user_id, QDateTime::currentMSecsSinceEpoch());
#else
    loaded.insert(user_id);
#endif
    m_log << QString("New user: %1 (%2)").arg(name, QString::number(birthDate));

#ifdef CHECK_DOUBLES
  } else {
    user_exists.value() = QDateTime::currentMSecsSinceEpoch();
#endif
  }

  auto contact = m_parser->value(csv_parser::fields::contact);
  contact.removeIf([](auto ch) { return ch == '"'; });

#ifdef CHECK_DOUBLES
  auto &users_contacts = m_ctx->users_contacts();
  auto user_contacts = users_contacts.find(user_id);
  if (user_contacts == users_contacts.end()) {
    user_contacts = users_contacts.insert(user_id, QSet<QString>{});
  }

  if (!user_contacts->contains(contact)) {
    user_contacts->insert(contact);
#else
  {
#endif
  }

  auto card = m_parser->value(csv_parser::fields::card).toULongLong();

#ifdef CHECK_DOUBLES
  auto &users_cards = m_ctx->users_cards();
  auto user_cards = users_cards.find(user_id);
  if (user_cards == users_cards.end()) {
    user_cards = users_cards.insert(user_id, QSet<quint64>{});
  }
  if (!user_cards->contains(card)) {
    user_cards->insert(card);
#else
  {
#endif

  m_fnSetPos(m_parser->position());
}
#ifdef CHECK_DOUBLES
void runnable_parser::_clear_old() {
  auto &users_contacts{m_ctx->users_contacts()};
  if (users_contacts.size() >= MEM_MAX_USERS_SIZE) {
    const auto &loaded{m_ctx->loaded()};

    m_log << "Clearing old user's data...";
    qint64 min = 0;
    qint64 max = 0;
    auto end = users_contacts.keyEnd();
    for (auto it = users_contacts.keyBegin(); it != end; ++it) {
      auto user_id = *it;
      const auto tstmp = loaded[user_id];
      if (min == 0 || tstmp < min) {
        min = tstmp;
      }
      if (max < tstmp) {
        max = tstmp;
      }
    }
    auto mid = min / 2 + max / 2;

    users_contacts.removeIf([&](auto &it) { return loaded[it.key()] <= mid; });
    m_ctx->users_cards().removeIf(
        [&](auto &it) { return loaded[it.key()] <= mid; });
  }

        m_ctx->container()->users.add(user_t{
            .id = user_id,
            .name = name,
            .birthDate = birthDate,
        });
            m_ctx->container()->contacts.add(contact_t{
                .user_id = user_id,
                .type = determineContactType(contact),
                .value = contact,
            });
            m_ctx->container()->cards.add(card_t{
                .user_id = user_id,
                .value = card,
                .expireDate = toDate(m_parser->value(csv_parser::fields::expiredate))});
        }
}
#endif
