spice
* dollar sign comment example
.options cut_inline_spice_comments

r1 1 0 10k
v1 1 0 dc 1             $ comment line 1
v2 2 0 			$ comment line 2
+      dc 2		$ comment line 3
+                       $ comment line 4

v3 3 0 			$ comment line 5 \
       dc 3

.list
.print op v(v*)
.op
.end
