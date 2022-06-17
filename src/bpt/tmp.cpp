struct FindOrders {
    string uid;
    int timestamp;

    FindOrders() : uid("~~~"), timestamp(114514) {};

    FindOrders(const string &uid, const int &timestamp) : uid(uid), timestamp(timestamp) {};

    FindOrders(const FindOrders &fs) : uid(fs.uid), timestamp(fs.timestamp) {};

    friend bool operator<(const FindOrders &l, const FindOrders &r) {
        if (r.uid == "~~~" && r.timestamp == 114514)return false;
        if (l.uid == "~~~" && l.timestamp == 114514)return true;
        if (l.uid == r.uid)return l.timestamp < r.timestamp;
        return l.uid < r.uid;
    }

    friend bool operator==(const FindOrders &l, const FindOrders &r) {
        return l.uid == r.uid && l.timestamp == r.timestamp;
    }
};

struct FindTickets {
    DATE d;
    string trainID;

    FindTickets() : d("99:99"), trainID("~~~") {};

    FindTickets(const DATE &d, const string &trainID) : d(d), trainID(trainID) {};

    FindTickets(const FindTickets &fs) : d(fs.d), trainID(fs.trainID) {};

    friend bool operator<(const FindTickets &l, const FindTickets &r) {
        if (l.d == "99:99" && l.trainID == "~~~") return true;
        if (r.d == "99:99" && l.trainID == "~~~") return false;
        if (l.trainID == r.trainID) {
            return l.d < r.d;
        }
        return l.trainID < r.trainID;
    }

    friend bool operator==(const FindTickets &l, const FindTickets &r) { return l.d == r.d && l.trainID == r.trainID; }
};

struct FindStations {
    string trainID;
    string stationID;

    FindStations() : stationID(""), trainID("") {};

    FindStations(const FindStations &fs) : stationID(fs.stationID), trainID(fs.trainID) {};

    FindStations(const string &l, const string &r) : stationID(l), trainID(r) {};

    friend bool operator<(const FindStations &l, const FindStations &r) {
        if (l.stationID == r.stationID) return l.trainID < r.trainID;
        return l.stationID < r.stationID;
    }

    friend bool operator==(const FindStations &l, const FindStations &r) {
        return l.stationID == r.stationID && l.trainID == r.trainID;
    }
};

struct pending{
    int timestamp,need,order1,order2;
    string userID,trainID;
};
struct FindPending{
    string trainID;
    DATE dptDate;
    int timestamp;
};

struct FindPD {
    string trainID;
    DATE dptDate;
};