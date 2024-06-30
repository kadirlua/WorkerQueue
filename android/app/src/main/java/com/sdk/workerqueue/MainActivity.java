package com.sdk.workerqueue;

import androidx.appcompat.app.AppCompatActivity;

import android.view.View;
import android.os.Bundle;
import android.widget.Button;
import java.util.ArrayList;

import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.content.Intent;
import android.net.Uri;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.documentfile.provider.DocumentFile;

import com.sdk.workerqueue.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'WorkerQueue' library on application startup.
    static {
        System.loadLibrary("WorkerQueueAndroid");
    }

    private ActivityMainBinding binding;

    private ArrayAdapter<String> adapter;
    private ArrayList<String> fileList;

    // Register the launcher for activity result
    private final ActivityResultLauncher<Intent> directoryPickerLauncher = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(),
            result -> {
                if (result.getResultCode() == RESULT_OK && result.getData() != null) {
                    Uri uri = result.getData().getData();
                    if (uri != null) {
                        DocumentFile directory = DocumentFile.fromTreeUri(this, uri);
                        if (directory != null && directory.isDirectory()) {
                            fileList.clear();
                            for (DocumentFile file : directory.listFiles()) {
                                fileList.add(file.getName());
                            }
                            adapter.notifyDataSetChanged();
                        }
                    }
                }
            }
    );

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        Button reqBtn = binding.buttonBrowse;
        ListView listViewDir = binding.listViewDir;

        fileList = new ArrayList<>();
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, fileList);
        listViewDir.setAdapter(adapter);

        reqBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
                directoryPickerLauncher.launch(intent);
            }
        });
    }

    /**
     * A native method that is implemented by the 'WorkerQueue' native library,
     * which is packaged with this application.
     */
    public native String[] listDirectoryContents(String path);
}