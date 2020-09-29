package bgu.spl.net.Frames.In;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Out.RECEIPT;
import bgu.spl.net.impl.stomp.DataBase;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;

public class DISCONNECT extends FrameIn {
    @Override
    public void execute(StompMessagingProtocolImpl protocol) {
        ConnectionsImpl<Frame> connections = protocol.getConnections();
        int id = protocol.getId();
        DataBase dataBase = DataBase.getInstance();
        dataBase.unRegister(id);
        RECEIPT receipt = new RECEIPT();
        receipt.addHeader("receipt-id",this.getHeader("receipt"));
        connections.send(id,receipt);
        dataBase.unRegister(id);
    }
}
