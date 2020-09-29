package bgu.spl.net.impl.stomp;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.In.FrameIn;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

import java.util.concurrent.atomic.AtomicInteger;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<Frame> {
    ConnectionsImpl<Frame> connections = null;
    //private final AtomicInteger id = new AtomicInteger(1);
    private int id = -1;
    final DataBase db;

    public StompMessagingProtocolImpl(DataBase db) {
        this.db = db;
    }


    @Override
    public void start(int connectionId, Connections<Frame> connections) {
        this.connections = (ConnectionsImpl<Frame>) connections;
        this.id = connectionId;
        db.initialConnections((ConnectionsImpl<Frame>)connections);
    }

    @Override
    public void process(Frame message) {
        FrameIn frame=(FrameIn)message;
        frame.execute(this);
    }

    @Override
    public boolean shouldTerminate() {
        return false;
    }

    public ConnectionsImpl<Frame> getConnections() {
        return connections;
    }

    public int getId() {
        return id;
    }
}
