package bgu.spl.net.Frames.In;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Out.RECEIPT;
import bgu.spl.net.impl.stomp.DataBase;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;

public class SUBSCRIBE extends FrameIn {
    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        ConnectionsImpl<Frame> connections = protocol.getConnections();
        int id = protocol.getId();
        DataBase dataBase = DataBase.getInstance();
        dataBase.addGenreID(this.getHeader("destination"),id + this.getHeader("id"),id);
        connections.addToChannel(this.getHeader("destination"), id);
        RECEIPT receipt = new RECEIPT();
        receipt.addHeader("receipt-id",this.getHeader("receipt"));
        connections.send(id,receipt);//maybe need to send in addition a MESSAGE type
    }
}
