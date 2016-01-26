{
  "targets": [
    {
      "target_name": "xorshift128plus",
      "sources": [ "src/xorshift128plus.cc" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
