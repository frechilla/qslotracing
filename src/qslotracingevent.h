#ifndef QSLOTRACINGEVENT_H
#define QSLOTRACINGEVENT_H

#include <QList>
#include <QTime>
#include <QtCore/QDebug>

/// Types of events to be passed to upper-most layers
typedef enum
{
    e_QSlotRacingEvent_Fuel = 0,   // fuel change
    e_QSlotRacingEvent_Lap,        // finish line crossed
    e_QSlotRacingEvent_Controller, // controller package
    e_QSlotRacingEvent_Ranking,    // ranking package
    e_QSlotRacingEvent_LapCounter, // lap counter package
    e_QSlotRacingEvent_Start,      // start package
    e_QSlotRacingEvent_Qualifying, // qualifying package
    e_QSlotRacingEvent_End         // end package
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

/// @brief controller event
class QSlotRacingEventController :
        public QSlotRacingEvent
{
private:
    /// type to save controller data per each player
    typedef struct
    {
        bool      valid;
        bool      lights;
        bool      lane_change;
        quint8    speed;
    } ControllerType_t;
    typedef QList< std::pair<QSlotRacingPlayer_t, ControllerType_t> > ControllerDataContainerType_t;
public:
    /// @param virtual time timestamp
    QSlotRacingEventController(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Controller, a_timestamp)
    {}

    ~QSlotRacingEventController()
    {}

    /// @brief store controller data for a specific player
    /// @param player whose controller data is to be stored
    /// @param controller data (byte containing lights, lane change and speed fields)
    void AddControllerData(QSlotRacingPlayer_t a_playersIndex, quint8 a_value)
    {
        ControllerType_t controller_data;

        // Check controller data
        if ((a_value & 0xC0) == 0xC0)
        {
            controller_data.valid = true;

            if (a_playersIndex == e_QSlotRacingPlayer1)
            {
            }

            // Check lights ON/OFF
            if ((a_value & 0x20) == 0x20)
            {
                // Lights OFF
                controller_data.lights = false;
            }
            else
            {
                // Lights ON
                controller_data.lights = true;
            }

            // Check lane change
            if ((a_value & 0x10) == 0x10)
            {
                // Lane change not pressed
                controller_data.lane_change = false;
            }
            else
            {
                // Lane change pressed
                controller_data.lane_change = true;
            }

            // Get speed value
            controller_data.speed = (a_value & 0x0F);
        }
        else
        {
            // Data is incorrect. Do nothing
            controller_data.valid = false;
        }

        // Store data
        m_controllerData.push_back(std::pair<QSlotRacingPlayer_t, ControllerType_t>(a_playersIndex, controller_data));
    }

    /// @return controller data of the player represented by 'a_playerIndex'
    ///         return a negative value if there was an error of any kind retrieving the controller data
    /// @param player's. If that player's data is not contained in this event
    ///        the function will return a negative value
    quint8 GetPlayersControllerData(QSlotRacingPlayer_t a_playerIndex, bool &valid, bool &lights, bool &lane_change, quint8 &speed) const
    {
        ControllerType_t controller_data;
        quint8           ret = -1;

        for (ControllerDataContainerType_t::const_iterator it = m_controllerData.begin();
             it != m_controllerData.end();
             it++)
        {
            if (it->first == a_playerIndex)
            {
                controller_data = it->second;

                // Get valid flag
                valid = controller_data.valid;

                // Get lights data
                lights = controller_data.lights;

                // Get lane change data
                lane_change = controller_data.lane_change;

                // Get speed data
                speed = controller_data.speed;

                ret = 0;
            }
        }

        // not found in controller data. Return a negative value
        return ret;
    }

private:
    /// contains player's index and its fuel value (from 0 to 100)
    ControllerDataContainerType_t m_controllerData;

    // prevent standard constructor from being used
    QSlotRacingEventController();
};


/// @brief ranking event
class QSlotRacingEventRanking :
        public QSlotRacingEvent
{
private:
    /// type to save ranking data per each player
    typedef struct
    {
        bool      moreThan15Laps;
        bool      carFlag;
        quint8    lapsBehind;
        quint8    car_id;
    } RankingType_t;
    typedef QList< std::pair<quint8, RankingType_t> > RankingDataContainerType_t;
public:
    /// @param virtual time timestamp
    QSlotRacingEventRanking(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Ranking, a_timestamp)
    {}

    ~QSlotRacingEventRanking()
    {}

    /// @brief store ranking data for a specific player
    /// @param ranking position
    /// @param ranking data
    void AddRankingData(quint8 pos, quint8 a_value)
    {
        RankingType_t ranking_data;

        // Check ranking data
        if (a_value == 0xFF)
        {
            // No car for this position
            ranking_data.carFlag = false;
        }
        else
        {
            // Set car flag
            ranking_data.carFlag = true;

            // Check for >15 laps behind
            if ((a_value & 0x80) == 0)
            {
                // More than 15 laps behind
                ranking_data.moreThan15Laps = true;

                // Get car ID
                ranking_data.car_id = (a_value & 0x07);
            }
            else
            {
                // Set no more than 15 laps behind
                ranking_data.moreThan15Laps = false;

                // Get number of laps behind
                ranking_data.lapsBehind = (a_value & 0x78) >> 3;

                // Get car ID
                ranking_data.car_id = (a_value & 0x07);
            }
        }

        // Store ranking data
        m_rankingData.push_back(std::pair<quint8, RankingType_t>(pos, ranking_data));
    }

    /// @return controller data of the player represented by 'a_playerIndex'
    ///         return a negative value if there was an error of any kind retrieving the controller data
    /// @param player's. If that player's data is not contained in this event
    ///        the function will return a negative value
    quint8 GetRankingDataByPos(quint8 pos, bool &moreThan15, bool &car_flag, quint8 &laps_behind, quint8 &car_id) const
    {
        RankingType_t ranking_data;
        quint8        ret = -1;

        for (RankingDataContainerType_t::const_iterator it = m_rankingData.begin();
             it != m_rankingData.end();
             it++)
        {
            if (it->first == pos)
            {
                ranking_data = it->second;

                // Get more than 15 laps behind flag
                moreThan15 = ranking_data.moreThan15Laps;

                // Get car flag
                car_flag = ranking_data.carFlag;

                // Get laps behind
                laps_behind = ranking_data.lapsBehind;

                // Get car ID
                car_id = ranking_data.car_id;

                ret = 0;
            }
        }

        // not found in controller data. Return a negative value
        return ret;
    }

    quint8 GetRankingDataByCar(quint8 car_id, quint8 &pos, bool &moreThan15, bool &car_flag, quint8 &laps_behind) const
    {
        RankingType_t ranking_data;
        quint8        ret = -1;
        bool          found;
        RankingDataContainerType_t::const_iterator it;

        // Initialization
        found = false;
        pos = 0;
        moreThan15 = true;
        car_flag = false;
        laps_behind = 0;

        it = m_rankingData.begin();
        while ((found != true) && (it != m_rankingData.end()))
        {
            // Check for car Id
            ranking_data = it->second;
            if (ranking_data.car_id == car_id)
            {
                // Car Id found. Set flag and return data
                found = true;
                ret = 0;

                pos = it->first;
                moreThan15 = ranking_data.moreThan15Laps;
                car_flag = ranking_data.carFlag;
                laps_behind = ranking_data.lapsBehind;
            }

            // Next item
            it++;
        }

        // not found in controller data. Return a negative value
        return ret;
    }

private:
    /// contains player's index and its fuel value (from 0 to 100)
    RankingDataContainerType_t m_rankingData;

    // prevent standard constructor from being used
    QSlotRacingEventRanking();
};

/// @brief Lap counter event
class QSlotRacingEventLapCounter :
        public QSlotRacingEvent
{
private:
    /// type to save ranking data per each player
    typedef struct
    {
        quint8    count_direction;
        quint32   laps;
    } LapCounterType_t;
public:
    /// @param virtual time timestamp
    QSlotRacingEventLapCounter(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_LapCounter, a_timestamp)
    {}

    ~QSlotRacingEventLapCounter()
    {}

    /// @brief store lap counter data
    /// @param lap counter data
    void AddLapCounterData(quint8 dir, quint8 byte2, quint8 byte1, quint8 byte0)
    {
        quint8 temp2;
        quint8 temp1;
        quint8 temp0;

        // Set lap counter direction
        m_LapCounterData.count_direction = dir;

        // Calculate laps
        temp2 = byte2 & 0x0F;
        temp1 = byte1 & 0x0F;
        temp0 = byte0 & 0x0F;

        m_LapCounterData.laps = (temp2 * 256) + (temp1 * 16) + temp0;
    }

    /// @return lap counter data
    ///         return a negative value if there was an error of any kind retrieving the controller data
    quint8 GetLapCounterData(quint8 &dir, quint32 &laps) const
    {
        quint8        ret = -1;

        // Return lap counter data
        dir = m_LapCounterData.count_direction;
        laps = m_LapCounterData.laps;
        ret = 0;

        return ret;
    }

private:
    /// contains player's index and its fuel value (from 0 to 100)
    LapCounterType_t m_LapCounterData;

    // prevent standard constructor from being used
    QSlotRacingEventLapCounter();
};

/// @brief Lap counter event
class QSlotRacingEventLap :
        public QSlotRacingEvent
{
private:
public:
    /// @param virtual time timestamp
    QSlotRacingEventLap(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Lap, a_timestamp)
    {}

    ~QSlotRacingEventLap()
    {}

    /// @brief store lap counter data
    /// @param lap counter data
    void AddLapData(QSlotRacingPlayer_t player, quint32 crossings, quint32 time)
    {
        // Set lap data
        m_player = player;
        m_crossingTimes = crossings;
        m_lapMillis = time;
    }

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

/// @brief Start package event
class QSlotRacingEventStart :
        public QSlotRacingEvent
{
private:
public:
    /// @param virtual time timestamp
    QSlotRacingEventStart(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Start, a_timestamp)
    {}

    ~QSlotRacingEventStart()
    {}

private:

    // prevent standard constructor from being used
    QSlotRacingEventStart();
};

/// @brief Qualifying package event
class QSlotRacingEventQualifying :
        public QSlotRacingEvent
{
private:
public:
    /// @param virtual time timestamp
    QSlotRacingEventQualifying(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_Qualifying, a_timestamp)
    {}

    ~QSlotRacingEventQualifying()
    {}

private:

    // prevent standard constructor from being used
    QSlotRacingEventQualifying();
};

/// @brief End package event
class QSlotRacingEventEnd :
        public QSlotRacingEvent
{
private:
public:
    /// @param virtual time timestamp
    QSlotRacingEventEnd(const QTime &a_timestamp):
            QSlotRacingEvent(e_QSlotRacingEvent_End, a_timestamp)
    {}

    ~QSlotRacingEventEnd()
    {}

private:

    // prevent standard constructor from being used
    QSlotRacingEventEnd();
};

#endif // QSLOTRACINGEVENT_H
