package bgu.spl.net.Frames.In;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Out.RECEIPT;
import bgu.spl.net.impl.stomp.DataBase;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;

public class UNSUBSCRIBE extends FrameIn {
    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        ConnectionsImpl<Frame> connections = protocol.getConnections();
        int id = protocol.getId();
        DataBase dataBase = DataBase.getInstance();
        String channel = dataBase.unSubscribe(this.getHeader("id"),id);
        connections.removeFromChannel(channel, id);
        RECEIPT receipt = new RECEIPT();
        receipt.addHeader("receipt-id",this.getHeader("id"));
        connections.send(id,receipt);
    }
}
