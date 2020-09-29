package bgu.spl.net.Frames.In;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Out.MESSAGE;
import bgu.spl.net.impl.stomp.DataBase;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;

public class SEND extends FrameIn {

    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        ConnectionsImpl<Frame> connections = protocol.getConnections();
        int userID = protocol.getId();
        MESSAGE message = new MESSAGE();
        String genre = this.getHeader("destination");
        DataBase dataBase = DataBase.getInstance();
        int subID = dataBase.getSubID(genre, userID);
        if (subID!=-1) {
            message.addHeader("subscription", Integer.toString(subID));
            message.addHeader("Message-id", Integer.toString(dataBase.getMessageID()));
            message.addHeader("destination", this.getHeader("destination"));
            message.addHeader("FrameBody", this.getHeader("FrameBody") + '\n');
            connections.send(this.getHeader("destination"), message);//check about the message John has Dune
        }
    }
}
