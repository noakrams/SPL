package bgu.spl.net.Frames.In;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Out.CONNECTED;
import bgu.spl.net.impl.stomp.DataBase;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;

public class CONNECT extends FrameIn {

    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        ConnectionsImpl<Frame> connections = protocol.getConnections();
        int id = protocol.getId();
        DataBase dataBase = DataBase.getInstance();
        dataBase.register(id, this.getHeader("login"), this.getHeader("passcode"));
        boolean success = dataBase.login(id, this.getHeader("passcode"), this, this.getHeader("login"));

        if (success) {
            CONNECTED connected = new CONNECTED();
            connected.addHeader("version", this.getHeader("accept-version"));
            connections.send(id, connected);
            dataBase.addLoggedID(this.getHeader("login"),id);
        }
    }
}
