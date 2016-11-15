package com.vintage.vintagechess;

import org.json.JSONException;

/**
 * Created by User on 2016-11-10.
 */

public interface RequestCallback {
    public void runResponse(String response) throws JSONException;
}
