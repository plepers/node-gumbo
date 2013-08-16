{
  "targets": [
    {
        "target_name": "gumbo",
        "sources": [ "src/gumbo.cc" ],
        "libraries": [
            '<!@(pkg-config --libs gumbo)'
        ],
        "include_dirs": [
            '<!@(pkg-config --cflags gumbo)'
        ]

    }
  ]
}