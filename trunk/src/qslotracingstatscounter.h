#ifndef QSLOTRACINGSTATSCOUNTER_H
#define QSLOTRACINGSTATSCOUNTER_H

#include <QtGlobal>     // qt types
#include "atomic_ops.h" // atomic increment and decrement

/// template-based wrapper for statistics tracking
/// It's a thread-safe class, though it doesn't support "snapshots", this is
/// at the same time a value is retrived another one might be getting updated
template <typename ENUM_TYPE, ENUM_TYPE NSTATS>
class QSlotRacingStatsCounter
{
public:
    /// type used to hoold the value of each stat entry
    typedef qint32 StatsCounterValueType_t;

    QSlotRacingStatsCounter()
    {
        for (int i = 0; i < NSTATS; i++)
        {
            m_stats[i].m_value = 0;
            m_stats[i].m_name = QString("");
        }
    }
    virtual ~QSlotRacingStatsCounter()
    {}

    /// @brief Add n to the stat counter pointed by 'a_index'
    void Increment(quint32 a_index, StatsCounterValueType_t n)
    {
        AtomicAdd(&(m_stats[a_index].m_value), n);
    }

    /// @brief Substract n to the stat counter pointed by 'a_index'
    void Decrement(quint32 a_index, StatsCounterValueType_t n)
    {
        AtomicSub(&(m_stats[a_index].m_value), n);
    }
    
    /// @return current value of a stat entry
    StatsCounterValueType_t GetEntryValue(quint32 a_index) const
    {
        return m_stats[a_index].m_value;
    }
    
    /// @brief set the string description of a specific stat entry
    void SetEntryTitle(quint32 a_index, const QString& a_newName)
    {
        m_stats[a_index].m_name = a_newName;
    }
    
    /// @return description of a specific stat entry
    const QString& GetEntryTitle(quint32 a_index) const
    {
        return m_stats[a_index].m_name;
    }

private:
    /// Fields kepr per stat entry
    typedef struct
    {
        volatile StatsCounterValueType_t m_value;
        QString m_name;
    } StatCounterEntry_t;

    /// array to keep track of the values
    StatCounterEntry_t m_stats[NSTATS];
    
    // no copy allowed for now
    QSlotRacingStatsCounter(const QSlotRacingStatsCounter& a_src);
    QSlotRacingStatsCounter& operator=(const QSlotRacingStatsCounter& a_src);
};

#endif // QSLOTRACINGSTATSCOUNTER_H
