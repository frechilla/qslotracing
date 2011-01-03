#ifndef QSLOTRACINGEVENT_H
#define QSLOTRACINGEVENT_H

#include <QList>

/// Types of events to be passed to the GUI
typedef enum
{
    // list of events handled by the app
    e_QSlotRacingEvent_Fuel = 0, // fuel change
} QSlotRacingEventType_t;


/// @brief base QSlotRacing event to be created by protocol parsers
/// To be inherited by real events. It can't be instantiated from outside of a children class
class QSlotRacingEvent
{
public:
    virtual ~QSlotRacingEvent()
    {}

    /// @return this event type
    /// can be used to perform safe downcasting
    inline QSlotRacingEventType_t EventType()
    {
        return m_eventType;
    }

protected:
    QSlotRacingEvent(QSlotRacingEventType_t a_eventType):
            m_eventType(a_eventType)
    {
    }

private:
    QSlotRacingEventType_t m_eventType;
};


/// @brief fuel event
/// Contains a list of changes related to the fuel of a list of cars
class QSlotRacingEventFuel :
        public QSlotRacingEvent
{
private:
    /// type to save amount of fuel per each player
    typedef QList< std::pair<quint8, quint8> > FuelDataContainerType_t;

public:
    QSlotRacingEventFuel():
            QSlotRacingEvent(e_QSlotRacingEvent_Fuel)
    {}
    ~QSlotRacingEventFuel()
    {}

    /// @brief store amount of fuel for a specific player
    /// @param index of that player whose fuel value is to be stored
    /// @param fuel value (from 0 to 100)
    void AddFuelData(quint8 m_playersIndex, quint8 a_value)
    {
        m_fuelData.push_back(std::pair<quint8, quint8>(m_playersIndex, a_value));
    }

    /// @return amount of fuel (from 0 to 100) of the player represented by 'a_playerIndex'
    ///         return a negative value if there was an error of any kind retrieving the fuel value
    /// @param player's index. If that player's data is not contained in this event
    ///
    quint8 GetPlayersFuel(quint8 m_playersIndex)
    {
        for (FuelDataContainerType_t::const_iterator it = m_fuelData.begin();
             it != m_fuelData.end();
             it++)
        {
            if (it->first == m_playersIndex)
            {
                return it->second;
            }
        }

        // not found in m_fuelData. Return a negative value
        return -1;
    }

private:
    /// contains player's index and its fuel value (from 0 to 100)
    FuelDataContainerType_t m_fuelData;
};

#endif // QSLOTRACINGEVENT_H
