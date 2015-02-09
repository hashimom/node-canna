{
  "targets": [
    {
      "target_name": "node_canna",
      "sources": [
        "src/mod_node_canna.cc",
        "src/nodecanna.cc",
        "src/cannasession.cc"
      ],
      "link_settings": {
        "libraries" : [
          "-lcanna"
        ]
      }
    }
  ]
}