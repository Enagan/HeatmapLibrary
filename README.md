///////////////////////////////////////////////////////
//             Heatmap Service Library               //
// Written by: Pedro Engana (http://pedroengana.com) //
///////////////////////////////////////////////////////

The HeatmapService library provides a robust and easy to use tool to log player activity spatially organized over 2d space.
It can log any counters on a coordinate basis to measure activity, such as deaths, gold collected, materials gathered, etc...

The Heatmap can be queried for it's stored values, from the smallest unit of space to the entirety of the record.
It can also serialize itself to a char* buffer and can later restored from the same buffer.

////// Solution Overview //////

The Heatmap Service Lib is constituted of 3 main components:

HeatmapService ---> Heatmap ---> CounterMap

- Heatmap Service: A container for the inner functionality of the library. 
It occludes implementation details from the library header, and provides the library API.

- Heatmap: The actual internal implementation of the Heatmap, 
It manages different CounterMaps for each counter (deaths, gold caught, etc...) it's responsible for incrementing the counters, querying and serializing itself and all it's maps.

- CounterMap: Is the nitty gritty map of accumulators. 
It implements a dynamically increasing matrix that can grow on Y independently for each column, and always provides access times of O(1) complexity




////// Discussion on Assumptions and Decisions //////

////// Future Work & What-ifs //////
