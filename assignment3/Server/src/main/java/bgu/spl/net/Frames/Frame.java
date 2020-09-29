package bgu.spl.net.Frames;

import java.util.LinkedHashMap;

public abstract class Frame {

    LinkedHashMap<String, String> headers = new LinkedHashMap<>();

    public LinkedHashMap<String, String> getHeaders() {
        return headers;
    }

    public void addHeader(String headerName_i, String headerValue_i) {
        headers.put(headerName_i, headerValue_i);
    }


    public String toString() {

        StringBuilder bobTheBuilder = new StringBuilder();
        bobTheBuilder.append(this.getClass().getSimpleName());
        bobTheBuilder.append("\n");

        headers.forEach((k, v) -> {
            bobTheBuilder.append(k).append(":").append(v);
            if(!"FrameBody".equals(k))
                bobTheBuilder.append("\n");
        });
        String str = new String(bobTheBuilder);
        str = str.replace("FrameBody:", "\n");

        return str;
    }

    ;

    public String getHeader(String s) {
        return headers.get(s);
    }

}
