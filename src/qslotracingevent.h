#ifndef QSLOTRACINGEVENT_H
#define QSLOTRACINGEVENT_H

#include <QList>
#include <QTime>

/// Types of events to be passed to upper-most layers
typedef enum
{
    e_QSlotRacingEvent_Fuel = 0, // fuel change
    e_QSlotRacingEvent_Controller, // controller package
    e_QSlotRacingEvent_Ranking, // ranking package
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

    e_QSlotRacingNoPlayer,
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
    /// constructor available for children classes
    QSlotRacingEvent(QSlotRacingEventType_t a_eventType,
                     const QTime &a_timestamp):
            m_eventType(a_eventType),
            m_timestamp(a_timestamp)
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
    /// @param virtual time timestamp
    QSlotRacingEventFuel(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Fuel, a_timestamp)
    {}
    ~QSlotRacingEventFuel()
    {}

    /// @brief store amount of fuel for a specific player
    /// @param player whose fuel value is to be stored
    /// @param fuel value (from 0 to 8)
    void AddFuelData(QSlotRacingPlayer_t a_playersIndex, quint8 a_value)
    {
        m_fuelData.push_back(std::pair<QSlotRacingPlayer_t, quint8>(a_playersIndex, a_value));
    }

    /// @return amount of fuel (from 0 to 8) of the player represented by 'a_playerIndex'
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


    // prevent standard constructor from being used
    QSlotRacingEventFuel();
};


/// @brief lap event
/// sent when a car crosses the finish line
class QSlotRacingEventLap :
        public QSlotRacingEvent
{
public:
    /// @param virtual time timestamp
    /// @param who triggered this lap event
    /// @param lap time. 0 if it is unknown
    QSlotRacingEventLap(const QTime         &a_timestamp,
                        QSlotRacingPlayer_t  a_who,
                        qint32               a_crossingTimes,
                        qint32               a_lapTimeMillis = -1):
            QSlotRacingEvent(e_QSlotRacingEvent_Fuel, a_timestamp),
            m_player(a_who),
            m_crossingTimes(a_crossingTimes),
            m_lapMillis(a_lapTimeMillis)
    {}
    ~QSlotRacingEventLap()
    {}

    /// @return who triggered this lap event
    inline QSlotRacingPlayer_t GetWho() const
    {
        return m_player;
    }

    /// @return number of times this player has crossed the finish line
    /// negative if unknown or bogus
    inline qint32 GetCrossingTimes() const
    {
        return m_crossingTimes;
    }

    /// @return lap time in milliseconds.
    /// negative if unknown or bogus. The first crossing (race start) might give a time of 0
    inline qint32 GetLapMillis() const
    {
        return m_lapMillis;
    }

private:
    /// @brief who triggered the event
    QSlotRacingPlayer_t m_player;

    /// @brief times this player has crossed the finish line
    qint32 m_crossingTimes;

    /// @brief lap time in milliseconds
    qint32 m_lapMillis;

    // prevent standard constructor from being used
    QSlotRacingEventLap();
};

/// @brief controller event
/// sent for each controller
class QSlotRacingEventController :
        public QSlotRacingEvent
{
private:
    /// type to save amount of fuel per each player
    typedef QList< std::pair<QSlotRacingPlayer_t, quint8> > ControllerDataContainerType_t;
public:
    /// @param virtual time timestamp
    QSlotRacingEventController(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Controller, a_timestamp)
    {}
    ~QSlotRacingEventController()
    {}

    /// @brief store amount of fuel for a specific player
    /// @param player whose fuel value is to be stored
    /// @param fuel value (from 0 to 8)
    void AddControllerData(QSlotRacingPlayer_t a_playersIndex, quint8 a_value)
    {
        m_controllerData.push_back(std::pair<QSlotRacingPlayer_t, quint8>(a_playersIndex, a_value));
    }

private:
    /// contains player's index and its fuel value (from 0 to 100)
    ControllerDataContainerType_t m_controllerData;

    // prevent standard constructor from being used
    QSlotRacingEventController();
};

/// @brief ranking event
/// sent after crossing the finish line
class QSlotRacingEventRanking :
        public QSlotRacingEvent
{
private:
    /// type to save amount of fuel per each player
    typedef QList< std::pair<QSlotRacingPlayer_t, quint8> > RankingDataContainerType_t;
public:
    /// @param virtual time timestamp
    QSlotRacingEventRanking(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Controller, a_timestamp)
    {}
    ~QSlotRacingEventRanking()
    {}

    /// @brief store ranking data for a specific player
    /// @param player whose ranking data is to be stored
    /// @param ranking data
    void AddRankingData(QSlotRacingPlayer_t a_playersIndex, quint8 a_value)
    {
        m_rankingData.push_back(std::pair<QSlotRacingPlayer_t, quint8>(a_playersIndex, a_value));
    }

private:
    /// contains player's index and its fuel value (from 0 to 100)
    RankingDataContainerType_t m_rankingData;

    // prevent standard constructor from being used
    QSlotRacingEventRanking();
};

#endif // QSLOTRACINGEVENT_H
