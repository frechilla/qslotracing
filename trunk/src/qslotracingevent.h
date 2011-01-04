#ifndef QSLOTRACINGEVENT_H
#define QSLOTRACINGEVENT_H

#include <QList>
#include <QTime>

/// Types of events to be passed to upper-most layers
typedef enum
{
    e_QSlotRacingEvent_Fuel = 0, // fuel change
} QSlotRacingEventType_t;

/// Players handled by this app
typedef enum
{
    e_QSlotRacingPlayer1 = 0,
    e_QSlotRacingPlayer2,
    e_QSlotRacingPlayer3,
    e_QSlotRacingPlayer4,
    e_QSlotRacingPlayer5,
    e_QSlotRacingPlayer6,

    // if more players are needed they are to be added here
} QSlotRacingPlayer_t;


/// @brief base QSlotRacing event to be created by protocol parsers
/// To be inherited by real events. It can't be instantiated from outside of a children class
class QSlotRacingEvent
{
public:
    virtual ~QSlotRacingEvent()
    {}

    /// @return this event type
    /// can be used to perform safe downcasting
    inline QSlotRacingEventType_t EventType() const
    {
        return m_eventType;
    }

    /// @return const reference to the creation time of this object
    /// returned timestamp is based on virtual time
    /// carried by sniffed messages (QSlotRacingMsg)
    inline const QTime& GetTimestamp() const
    {
        return m_timestamp;
    }

protected:
    QSlotRacingEvent(QSlotRacingEventType_t a_eventType):
            m_eventType(a_eventType)
    {
    }

private:
    /// @brief event type
    QSlotRacingEventType_t m_eventType;

    /// @brief timestamp
    QTime m_timestamp;


    // prevent standard constructor from being used
    QSlotRacingEvent();
};


/// @brief fuel event
/// Contains a list of changes related to the fuel of a list of cars
class QSlotRacingEventFuel :
        public QSlotRacingEvent
{
private:
    /// type to save amount of fuel per each player
    typedef QList< std::pair<QSlotRacingPlayer_t, quint8> > FuelDataContainerType_t;

public:
    QSlotRacingEventFuel():
            QSlotRacingEvent(e_QSlotRacingEvent_Fuel)
    {}
    ~QSlotRacingEventFuel()
    {}

    /// @brief store amount of fuel for a specific player
    /// @param player whose fuel value is to be stored
    /// @param fuel value (from 0 to 100)
    void AddFuelData(QSlotRacingPlayer_t a_playersIndex, quint8 a_value)
    {
        m_fuelData.push_back(std::pair<QSlotRacingPlayer_t, quint8>(a_playersIndex, a_value));
    }

    /// @return amount of fuel (from 0 to 100) of the player represented by 'a_playerIndex'
    ///         return a negative value if there was an error of any kind retrieving the fuel value
    /// @param player's. If that player's data is not contained in this event
    ///        the function will return a negative value
    quint8 GetPlayersFuel(QSlotRacingPlayer_t a_playerIndex) const
    {
        for (FuelDataContainerType_t::const_iterator it = m_fuelData.begin();
             it != m_fuelData.end();
             it++)
        {
            if (it->first == a_playerIndex)
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
