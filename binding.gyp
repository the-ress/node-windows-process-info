{
  "targets": [{
    "target_name": "process-info",
    'include_dirs': [
      '<!(node -e "require(\'nan\')")'
    ],
    "sources": [
      "src/process-info.cc"
    ]
  }]
}