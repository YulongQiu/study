package com.example.lony.hellomoon;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

/**
 * Created by Lony on 2015/11/11.
 */
public class HelloMoonFragment extends Fragment {

    private Button mPlayButton;
    private Button mStopButton;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_hello_moon, container, false);

        mPlayButton = (Button)view.findViewById(R.id.hellomoon_playButton);
        mStopButton = (Button)view.findViewById(R.id.hellomoon_stopButton);
        Log.d("HelloMoonFragment", "onCreateView");
        return view;
    }
}
