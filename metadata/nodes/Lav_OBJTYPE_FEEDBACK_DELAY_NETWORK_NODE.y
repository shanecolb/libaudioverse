properties:
  Lav_FDN_INTERPOLATION_TIME:
    name: interpolation_time
    type: float
    range: [0.001, INFINITY]
    default: 0.001
    doc_description: |
      The crossfade duration used for the internal delay lines of this network.
  Lav_FDN_MAX_DELAY:
    name: delay_max
    type: float
    read_only: true
    doc_description: |
      The maximum delay any of the internal delay lines may be set to.
extra_functions:
  Lav_feedbackDelayNetworkNodeSetOutputGains:
    doc_description: |
      Set the gains of the individual output channels.
    params:
      count: The number of values.  Must match the number of internal delay lines.
      values: The new gains.
  Lav_feedbackDelayNetworkNodeSetDelays:
    doc_description: |
      Set the internal delays of the FDN.
    params:
      count: The number of delays.  This must match the FDN's internal delay count as set by the constructor.
      values: The new values for the delays.
  Lav_feedbackDelayNetworkNodeSetFeedbackDelayMatrix:
    doc_description: |
      Set the FDN's feedback matrix.
    params:
      count: The number of values.  Must be exactly the product of the feedback matrix, that is the number of delay lines squared.
      values: The new feedback matrix, stored in row-major order.
inputs:
  - [constructor, "The input to the FDN."]
outputs:
  - [constructor, "The output of the FDN."]
doc_name: feedback delay network
doc_description: |
  Implements a feedback delay network.
  This is possibly the single-most complicated node in Libaudioverse, and full documentation of it goes well beyond the manual.
  Unless you know  what a  feedback delay network is and have a specific reason for using one, this node will probably not help you.