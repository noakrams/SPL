package bgu.spl.net.impl.stomp;

import bgu.spl.net.Frames.In.FrameIn;
import bgu.spl.net.Frames.Frame;
import bgu.spl.net.api.MessageEncoderDecoder;

import java.lang.reflect.Constructor;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.concurrent.atomic.AtomicInteger;

public class MessageEncoderDecoderImpl implements MessageEncoderDecoder<Frame> {
    private LinkedList<Byte> bytes = new LinkedList<Byte>(); //starts with 1k

    public Frame decodeNextByte(byte nextByte) {
        if (nextByte == '\0') {
            String n = popString();
            bytes.clear();
            return makeFrame(n);
        }
        bytes.add(nextByte);
        return null; //not a line yet
    }

    public byte[] encode(Frame message) {
        return (message + "\0").getBytes(StandardCharsets.UTF_8); //uses utf8 by default
    }

    @SuppressWarnings("unchecked")
    private Frame makeFrame(String message) {
        Frame frame = null;
        try {
            LinkedList<String> brokenMessage = getLines(message);
            String command = brokenMessage.removeFirst();
            Class<? extends FrameIn> type = (Class<? extends FrameIn>) Class.forName("bgu.spl.net.Frames.In." + command);
            Constructor<? extends FrameIn> constructor = type.getConstructor();
            frame = constructor.newInstance();

            for (String line : brokenMessage) {
                String[] keyVal = line.split(":", 2);
                String key = keyVal[0];
                String val = keyVal[1];

                frame.addHeader(key, val);
            }
            System.out.println(frame.toString());
        } catch (Exception e) {
        }
        return frame;
    }

    private LinkedList<String> getLines(String message) {
        message = message.replace("\n\n", "\nFrameBody:");
        String[] linesArr = message.split("\n");
        LinkedList<String> lines_list = new LinkedList<>(Arrays.asList(linesArr));
        return lines_list;
    }


    private String popString() {
        byte[] bytesArr = new byte[bytes.size()];
        final AtomicInteger i = new AtomicInteger(0);
        bytes.forEach(b -> bytesArr[i.getAndIncrement()] = b);
        return new String(bytesArr, StandardCharsets.UTF_8);
    }
}
